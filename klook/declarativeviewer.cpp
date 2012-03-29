/* KLook
 * Copyright (c) 2011-2012 ROSA  <support@rosalab.ru>
 * Authors: Julia Mineeva, Evgeniy Auzhin, Sergey Borovkov.
 * License: GPLv3
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 3,
 *   or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "declarativeviewer.h"

#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>

#include <kmimetypetrader.h>

#include "video.h"
#include "text.h"
#include "previewgenerator.h"
#include "previewprovider.h"
#include "filemodel.h"
#include "workerthread.h"

#include <QX11Info>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

static int header_height = 27;
static int border_width = 2;
static int height_offset = 27;

DeclarativeViewer::DeclarativeViewer(const QStringList& params, QWidget* parent )
    : QDeclarativeView( parent )
    , m_lastMousePosition( 0, 0 )
    , m_isSingleMode( true )
    , m_moving( false )
    , m_resize( false )
    , m_startFullScreen( false )
    , m_isEmbedded( false )
    , m_isGallery( false )
    , m_currentFile( 0 )
    , m_region( FRAME_REGION )
    , m_mediaObject( 0 )
    , m_videoWidget( 0 )
    , m_width( 600 )
    , m_height( 427 )
    , m_thread( 0 )
{
    setOptimizationFlags( QGraphicsView::DontSavePainterState );
    setViewportUpdateMode( QGraphicsView::BoundingRectViewportUpdate);

    setAttribute( Qt::WA_TranslucentBackground );
    setStyleSheet( "background:transparent;" );

    setMouseTracking( true );

    connect( engine(), SIGNAL( quit() ), SLOT( close() ) );

    engine()->addImageProvider( "preview", new PreviewProvider );

    setResizeMode( QDeclarativeView::SizeRootObjectToView );

    setMinimumWidth( m_width );
    setMinimumHeight( m_height );

    // Process parameters
    processArgs( params );

    m_previewGenerator = PreviewGenerator::createInstance();
    m_previewGenerator->setFiles( m_urls );

    ListItem *prototypeItem = new ListItem( this );
    m_fileModel = new FileModel( prototypeItem, this );
    PreviewGenerator::createInstance()->setModel(m_fileModel);

    connect( this, SIGNAL( fileData( QVariant, QVariant ) ), m_fileModel, SLOT( append( QVariant, QVariant ) ) );

    setRegisterTypes();

    //Check whether the KDE effects are included
    QDBusInterface remoteApp( "org.kde.kwin", "/KWin", "org.kde.KWin" );
    QDBusReply<bool> reply = remoteApp.call( "compositingActive" );
    if ( reply.isValid() )
    {
        if (reply.value())
            rootContext()->setContextProperty( "effects", "on" );
        else
            rootContext()->setContextProperty( "effects", "off" );
    }
    else
    {
        rootContext()->setContextProperty( "effects", "off" );
        qDebug() << "DBus reply is not valid";
    }

    //Remove standart KDE title
    setWindowFlags( Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    //Skip Taskbar
    Display* dpy = QX11Info::display();
    Atom state[3];
    state[0] = XInternAtom( dpy, "_NET_WM_STATE_SKIP_PAGER", false );
    state[1] = XInternAtom( dpy, "_NET_WM_STATE_SKIP_TASKBAR", false );
    state[2] = XInternAtom( dpy, "_NET_WM_STATE_STICKY", false );
    XChangeProperty( QX11Info::display(), winId(), XInternAtom( dpy, "_NET_WM_STATE", False ), XA_ATOM, 32, PropModeReplace, ( unsigned char* )state, 3 );

    setViewMode( Single );

    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)),this,SLOT(focusChanged(QWidget*,QWidget*)));
}

DeclarativeViewer::~DeclarativeViewer()
{
    qDeleteAll( m_files );

    delete m_thread;
}

void DeclarativeViewer::setRegisterTypes()
{
    qmlRegisterType<MyVideo>( "Widgets", 1, 0, "Video" );
    qmlRegisterType<MyText>( "Widgets", 1, 0, "PlainText" );

    QDesktopWidget dw;
    QRect r = dw.screenGeometry( this );
    rootContext()->setContextProperty( "DWigth", r.width() );
    rootContext()->setContextProperty( "DHeight", r.height() );
    rootContext()->setContextProperty( "fileModel", m_fileModel );
    rootContext()->setContextProperty( "previewGenerator", PreviewGenerator::createInstance() );
    rootContext()->setContextProperty( "cppObject",  this );
    rootContext()->setContextProperty( "effects", "on" );
    rootContext()->setContextProperty( "actualSize", "off" );
    rootContext()->setContextProperty( "openText", ki18n( "Open in..." ).toString() );
    rootContext()->setContextProperty( "fileName", "" );
    rootContext()->setContextProperty( "fileUrl", "" );
    rootContext()->setContextProperty( "fileType", "Undefined" );
    rootContext()->setContextProperty( "viewMode", "single" );
    rootContext()->setContextProperty( "embedded", m_isEmbedded );
}

void DeclarativeViewer::startWorkingThread()
{
    delete m_thread;
    m_thread = new WorkerThread( m_urls );

    connect( m_thread, SIGNAL( fileProcessed( const File* ) ), SLOT( newFileProcessed( const File* ) ) );
    connect( m_thread, SIGNAL( fail() ), SLOT( showNoFilesNotification() ) );

    m_thread->start();
}

void DeclarativeViewer::createVideoObject( const QString& filePath )
{
    delete m_videoWidget;
    delete m_mediaObject;

    m_mediaObject = new Phonon::MediaObject();
    m_mediaObject->setCurrentSource( QUrl( filePath ) );
    m_videoWidget = new Phonon::VideoWidget();
    m_videoWidget->hide();

    Phonon::createPath( m_mediaObject, m_videoWidget );
    QObject::connect( m_mediaObject, SIGNAL( metaDataChanged() ), this, SLOT( onMetaDataChanged() ) );
}

void DeclarativeViewer::onMetaDataChanged()
{
    if ( m_videoWidget )
    {
        m_width = m_videoWidget->sizeHint().width();
        m_height = m_videoWidget->sizeHint().height();

        delete m_videoWidget;
        m_videoWidget = 0;

        QSize sz = calculateViewSize( QSize( m_width, m_height ) );
        sz.setHeight( sz.height() + ( m_isEmbedded ? 0 : height_offset ) );

        if ( ( m_startFullScreen ) && ( sz == size() ) )
        {
            showFullScreen();
            emit setFullScreenState();
        }
        else
        {
            centerWidget( sz );
        }
    }

    m_startFullScreen = false;
    if ( m_mediaObject )
    {
        QObject::disconnect( m_mediaObject, SIGNAL( metaDataChanged() ), this, SLOT( onMetaDataChanged() ) );

        delete m_mediaObject;
        m_mediaObject = 0;
    }
}

void DeclarativeViewer::setActualSize()
{
    updateSize( m_currentFile );
}

void DeclarativeViewer::setActualSizeParam( bool value )
{
    Q_UNUSED( value )
}

void DeclarativeViewer::setFullScreen()
{
    if ( isFullScreen() )
    {
        showNormal();
        setActualSize();
    }
    else if ( ( geometry().size() == getActualSize() ) || m_isGallery )
    {
        showFullScreen();
        emit setFullScreenState();
    }
    else
    {
        m_startFullScreen = true;
        setActualSize();
    }
}

QSize DeclarativeViewer::getActualSize()
{    
    if ( m_currentFile == 0 )
    {
        return QSize();
    }
    if ( m_currentFile->type() == File::Video )
    {
        return QSize();
    }
    else if ( m_currentFile->type() == File::Image )
    {
        QPixmap pixmap( m_currentFile->name() );
        m_width = pixmap.width();
        m_height = pixmap.height();
        QSize sz = calculateViewSize( QSize( m_width, m_height ) );
        sz.setHeight( sz.height() + ( m_isEmbedded ? 0 : height_offset ) );
        return sz;
    }
    else
    {
        QSize size = getTextWindowSize(m_currentFile->name());
        return size;
    }

    return QSize();
}

QSize DeclarativeViewer::calculateViewSize( const QSize& sz )
{
    QSize szItem = sz;
    QDesktopWidget dw;
    QRect rectDesktop = dw.screenGeometry( this );

    int wDesktop = rectDesktop.width() * 8 / 10;
    int hDesktop = rectDesktop.height() * 8 / 10;

    szItem = inscribedRectToRect( sz, QSize( wDesktop, hDesktop ) );

    szItem.setWidth( ( szItem.width() < minimumWidth() ) ? minimumWidth() : szItem.width() ) ;
    szItem.setHeight( (szItem.height() < minimumHeight() ) ? minimumHeight() : szItem.height() );

    return szItem;
}

// function inscribing rect sz1 into rect sz2
QSize DeclarativeViewer::inscribedRectToRect( const QSize& sz1, const QSize& sz2 )
{
    QSize sz = sz1;

    if ( ( sz.height() > sz2.height() ) || ( sz.width() > sz2.width() ) )
    {
        sz.scale( sz2, Qt::KeepAspectRatio );
    }

    return sz;
}

void DeclarativeViewer::updateSize( const File* file )
{    
    if ( file == 0 )
    {
        m_startFullScreen = false;
        return;
    }

    if ( file->type() == File::Video )
    {
        createVideoObject( file->name() );
    }
    else if ( file->type() == File::Image )
    {
        QPixmap pixmap( file->name() );

        m_width = pixmap.width();
        m_height = pixmap.height();

        QSize sz = calculateViewSize( QSize( m_width, m_height ) );
        sz.setHeight( sz.height() + ( m_isEmbedded ? 0 : height_offset ) );

        if ( ( m_startFullScreen ) && ( sz == size() ) )
        {
            showFullScreen();
            emit setFullScreenState();
        }
        else
        {
            centerWidget( sz );
        }
        m_startFullScreen = false;
    }
    else if ( file->type() == File::Txt )
    {
        QSize size = getTextWindowSize(file->name());
        if ((m_startFullScreen) && (size == this->size()))
        {
            showFullScreen();
            emit setFullScreenState();
        }
        else
        {
            centerWidget( size );
        }
        m_startFullScreen = false;
    }    
}

void DeclarativeViewer::centerWidget( const QSize& sz )
{
    QDesktopWidget dw;
    QRect rectDesktop = dw.screenGeometry(this);
    QSize sz1;
    if (m_isEmbedded)
    {
        int iconOffset = 30;
        QRect top(rectDesktop.x() + rectDesktop.width()*0.1 ,
                  rectDesktop.y() + rectDesktop.height()*0.1 ,
                  rectDesktop.width()*0.8 ,
                  m_rcIcon.y() - rectDesktop.height()*0.1 - iconOffset );

        QRect left(rectDesktop.x() + rectDesktop.width()*0.1,
                   rectDesktop.y() + rectDesktop.height()*0.1,
                   m_rcIcon.x() - iconOffset - rectDesktop.x() - rectDesktop.width()*0.1,
                   rectDesktop.height()*0.8);

        QRect right(m_rcIcon.topRight().x(),
                    rectDesktop.y() + rectDesktop.height()*0.1,
                    rectDesktop.width() - m_rcIcon.topRight().x() - rectDesktop.width()*0.1,
                    rectDesktop.height()*0.8);

        qDebug() << top << left << right;

        int topArea = top.width()*top.height();
        int leftArea = left.width()*left.height();
        int rightArea = right.width()*right.height();
/*
        QWidget * wid = new QWidget();
        wid->setGeometry(m_rcIcon);
        wid->show();
*/

        if ((topArea > leftArea)&&(topArea > rightArea))
        {
            sz1 = inscribedRectToRect( sz, top.size() );            
            int x = m_rcIcon.x() + m_rcIcon.width()/2 - sz1.width()/2;
            x = qMax(x , static_cast<int>(rectDesktop.width()*0.1));
            x = qMin(x, top.topRight().x() - sz1.width());
            int y = m_rcIcon.y() - iconOffset - sz1.height();            
            QRect rect(x,y,sz1.width(),sz1.height());
            setGeometry(rect);
        }
        else if (leftArea > rightArea )
        {
            sz1 = inscribedRectToRect( sz, left.size() );
            int x = m_rcIcon.x() - sz1.width() - iconOffset;
            int y = m_rcIcon.y() + m_rcIcon.height()/2 - sz1.height()/2;
            y = qMax(y , static_cast<int>(rectDesktop.height()*0.1));
            y = qMin(y, left.bottomLeft().y() -sz1.height() );
            QRect rect(x,y,sz1.width(),sz1.height());
            setGeometry(rect);
        }
        else
        {
            sz1 = inscribedRectToRect( sz, right.size() );
            qDebug() << sz1;
            int x = m_rcIcon.topRight().x() + iconOffset;
            int y = m_rcIcon.y() + m_rcIcon.height()/2 - sz1.height()/2;
            y = qMax(y , static_cast<int>(rectDesktop.height()*0.1));
            y = qMin(y, right.bottomLeft().y() - sz1.height() );
            QRect rect(x,y,sz1.width(),sz1.height());
            qDebug() << rect;
            setGeometry(rect);
        }        
    }
    else
    {
        int w = sz.width();
        int h = sz.height();

        QRect rect( ( rectDesktop.width() - w ) / 2,
                    ( rectDesktop.height() - h ) / 2,
                    w, h );
        rect.moveTop( rect.y() - height_offset / 2 );

        setGeometry( rect );
    }
}

void DeclarativeViewer::changeContent()
{
    if ( !m_currentFile  ) return;

    ViewMode mode = ( m_files.size() == 1 ) ? Single : Multi;
    setViewMode( mode );

    KService::Ptr ptr = KMimeTypeTrader::self()->preferredService( m_currentFile->mime() );
    if ( ptr.isNull() )
        rootContext()->setContextProperty( "openText", ki18n( "Open" ).toString() );
    else
    {
        KService *serv = ptr.data();
        rootContext()->setContextProperty( "openText",  ( ki18n( "Open in " ).toString() + serv->name() ) );
    }

    QFileInfo fi( m_currentFile->name() );
    rootContext()->setContextProperty( "fileName", fi.fileName() );
    rootContext()->setContextProperty( "fileUrl", m_currentFile->name() );
    rootContext()->setContextProperty( "fileType", File::fileTypeToString( m_currentFile->type() ) );
}

void DeclarativeViewer::updateContent( int index )
{
    if (index == -1)
    {
        rootContext()->setContextProperty( "openText",  ki18n( "Open in..." ).toString() );
        rootContext()->setContextProperty( "fileName",  ki18n( "Elements: " ).toString() + QString::number( m_files.count() ) );
    }
    else
    {
        m_currentFile = m_files.at( index );
        changeContent();
    }
}

WidgetRegion DeclarativeViewer::calculateWindowRegion( const QPoint& mousePos )
{
    QPointF pos;
    pos = mousePos;
    QRectF r = rect();
    QRectF headerTitle;
    QRectF header1( 6 + 42 * 2, border_width + 1, 60, header_height - border_width + 1 );
    QRectF header2( r.width() - 42 * 2 - 12 - 6, border_width + 1, 12, header_height- border_width + 1 );
    QRegion headerR;

    if ( m_isSingleMode )
    {
        headerTitle.setRect( border_width + 1,
                             border_width + 1,
                             r.width() - border_width - 270,
                            header_height - border_width + 1 );
        headerR = headerR.united( headerTitle.toRect() );
        headerR = headerR.united( header2.toRect() );
    }
    else
    {
        headerTitle.setRect( 6 + 42 * 3 + 12,
                            border_width + 1,
                            r.width() - 414,
                            header_height- border_width + 1 );
        headerR = headerR.united( headerTitle.toRect() );
        headerR = headerR.united( header1.toRect() );
        headerR = headerR.united( header2.toRect() );
    }

    QRectF topBorder( 0, 0, r.width(), border_width );
    QRectF leftBorder( 0, 0, border_width, r.height() );
    QRectF rightBorder( r.width()-border_width, 0, border_width, r.height() );
    QRectF bottomBorder( 0, r.height()-border_width, r.width(), border_width );

    if ( headerR.contains( pos.toPoint() ) )
    {
        return HEADER_REGION;
    }
    else if ( topBorder.contains( pos ) )
    {
        if ( leftBorder.contains( pos ) )
        {
            return TOP_LEFT_CORNER_REGION;
        }
        else if ( rightBorder.contains( pos ) )
        {
            return TOP_RIGHT_CORNER_REGION;
        }
        else
        {
            return TOP_BORDER_REGION;
        }
    }
    else if ( bottomBorder.contains( pos ) )
    {
        if ( leftBorder.contains( pos ) )
        {
            return BOTTOM_LEFT_CORNER_REGION;
        }
        else if ( rightBorder.contains( pos ) )
        {
            return BOTTOM_RIGHT_CORNER_REGION;
        }
        else
        {
            return BOTTOM_BORDER_REGION;
        }
    }
    else if ( leftBorder.contains( pos ) )
    {
        return LEFT_BORDER_REGION;
    }
    else if ( rightBorder.contains( pos ) )
    {
        return RIGHT_BORDER_REGION;
    }

    return FRAME_REGION;
}

void DeclarativeViewer::mousePressEvent( QMouseEvent* event )
{
    if ( !isFullScreen() )
    {
        m_region = calculateWindowRegion( event->pos() );
        if ( event->button() == Qt::LeftButton )
        {
            if ( m_region == HEADER_REGION )
            {
                m_moving = true;
                m_lastMousePosition = event->globalPos();
                setCursor( QCursor( Qt::SizeAllCursor ) );
            }
            else if ( m_region != FRAME_REGION )
            {
                m_resize = true;
                m_lastMousePosition = event->globalPos();
            }
        }
        event->accept();
    }
    QDeclarativeView::mousePressEvent(event);
}

void DeclarativeViewer::mouseMoveEvent( QMouseEvent* event )
{
    QDeclarativeView::mouseMoveEvent( event );
    if ( isFullScreen() )
    {
        return;
    }

    if ( m_isEmbedded )
    {
        return;
    }

    if ( event->buttons().testFlag( Qt::LeftButton ) && m_moving )
    {
        if ( viewport()->cursor().shape() != ( Qt::SizeAllCursor ) )
            viewport()->setCursor( QCursor( Qt::SizeAllCursor ) );
        window()->move( window()->pos() + ( event->globalPos() - m_lastMousePosition ) );
        m_lastMousePosition = event->globalPos();
    }
    else if ( event->buttons().testFlag( Qt::LeftButton ) && m_resize )
    {
        QPoint p = mapToGlobal( event->pos()) - geometry().topLeft();
        QPoint offset = event->globalPos() - m_lastMousePosition;
        switch ( m_region )
        {
        case LEFT_BORDER_REGION:
            if ( offset.x() * ( -1 ) + width() > minimumWidth() )
            {
                window()->move( ( window()->pos() + offset ).x(), y() );
                resize( offset.x() * ( -1 ) + width(), height() );
                m_lastMousePosition = event->globalPos();
            }
            break;
        case TOP_BORDER_REGION:
            if (offset.y()*(-1) + height() > minimumHeight())
            {
                window()->move( x(), (window()->pos() + offset).y() );
                resize( width(), offset.y() * ( -1 ) + height() );
                m_lastMousePosition = event->globalPos();
            }
            break;
        case TOP_LEFT_CORNER_REGION:
            if ( offset.x() * ( -1 ) + width() > minimumWidth() )
            {
                window()->move( ( window()->pos() + offset).x(), y() );
                resize( offset.x() * ( -1 ) + width(), height() );
                m_lastMousePosition.setX( event->globalPos().x() );
            }
            if ( offset.y() * ( -1 ) + height() > minimumHeight() )
            {
                window()->move( x(), ( window()->pos() + offset ).y() );
                resize( width(), offset.y() * ( -1 ) + height() );
                m_lastMousePosition.setY( event->globalPos().y() );
            }
            break;
        case TOP_RIGHT_CORNER_REGION:
            resize( p.x(), height() );
            if ( offset.y() * ( -1 ) + height() > minimumHeight() )
            {
                window()->move( x(), ( window()->pos() + offset ).y() );
                resize( width(), offset.y() * ( -1 ) + height() );
                m_lastMousePosition.setY( event->globalPos().y() );
            }
            break;
        case BOTTOM_LEFT_CORNER_REGION:
            resize( width() , p.y() );
            if ( offset.x() * ( -1 ) + width() > minimumWidth() )
            {
                window()->move( ( window()->pos() + offset ).x(), y() );
                resize( offset.x() * ( -1 ) + width(), height() );
                m_lastMousePosition.setX( event->globalPos().x() );
            }
            break;
        case RIGHT_BORDER_REGION:
            resize( p.x(), height() );
            break;
        case BOTTOM_BORDER_REGION:
            resize( width() , p.y() );
            break;
        default:
            resize( p.x(), p.y() );
        }
    }
    else
    {
        m_region = calculateWindowRegion( event->pos() );
        if ( ( m_region == TOP_BORDER_REGION ) || ( m_region == BOTTOM_BORDER_REGION ) )
        {
            if ( viewport()->cursor().shape() != ( Qt::SizeVerCursor ) )
            {
                viewport()->setCursor( QCursor(Qt::SizeVerCursor) );
            }
        }
        else if ( ( m_region == LEFT_BORDER_REGION ) || ( m_region == RIGHT_BORDER_REGION ) )
        {
            if ( viewport()->cursor().shape() != ( Qt::SizeHorCursor ) )
            {
                viewport()->setCursor( QCursor(Qt::SizeHorCursor ) );
            }
        }
        else if ( ( m_region == TOP_LEFT_CORNER_REGION ) || ( m_region == BOTTOM_RIGHT_CORNER_REGION ) )
        {
            if ( viewport()->cursor().shape() != ( Qt::SizeFDiagCursor ) )
            {
                viewport()->setCursor( QCursor( Qt::SizeFDiagCursor ) );
            }
        }
        else if ( ( m_region == TOP_RIGHT_CORNER_REGION ) || ( m_region == BOTTOM_LEFT_CORNER_REGION ) )
        {
            if ( viewport()->cursor().shape() != ( Qt::SizeBDiagCursor ) )
            {
                viewport()->setCursor( QCursor( Qt::SizeBDiagCursor ) );
            }
        }
        else if ( viewport()->cursor().shape() != ( Qt::ArrowCursor ) )
        {
            viewport()->setCursor( QCursor( Qt::ArrowCursor ) );
        }
    }
}

void DeclarativeViewer::mouseReleaseEvent( QMouseEvent* event )
{
    if ( !isFullScreen() )
    {
        if ( m_moving )
        {
            m_moving = false;
            viewport()->setCursor( QCursor( Qt::ArrowCursor ) );
        }

        if ( m_resize )
        {
            m_resize = false;
            viewport()->setCursor( QCursor( Qt::ArrowCursor ) );
        }
        event->accept();
    }
    QDeclarativeView::mouseReleaseEvent( event );
}

void DeclarativeViewer::newFileProcessed( const File *file )
{
    if ( m_files.empty() )
    {
        m_currentFile = const_cast<File *>( file );
        changeContent();
        setActualSize();
        show();
    }
    else {
        setViewMode( Multi );
    }

    m_files.append( file );
    emit fileData( QVariant( file->name() ), QVariant( file->type() ) );
}

void DeclarativeViewer::showNoFilesNotification()
{
    close();
}

void DeclarativeViewer::setViewMode( DeclarativeViewer::ViewMode mode )
{
    QString modeName;
    if ( mode == Single )
    {
        modeName = "single";
        m_isSingleMode = true;
    }
    else
    {
        modeName = "multi";
        m_isSingleMode = false;
    }
    rootContext()->setContextProperty( "viewMode", modeName );
}

void DeclarativeViewer::handleMessage( const QString& message )
{
    QStringList params = message.split( ";", QString::SkipEmptyParts );
    processArgs( params );

    m_files.clear();

    startWorkingThread();

    m_fileModel->reset();

    m_previewGenerator->setFiles( m_urls );

    rootContext()->setContextProperty( "viewMode", ( ( m_urls.count() == 1 ) ? "single" : "multi" ) );

    emit setStartWindow();
}

int DeclarativeViewer::processArgs( const QStringList& args )
{
    setEmbedded(false);
    m_urls.clear();

    for ( int n = 0; n < args.count(); n++ )
    {
        QString argument = args[ n ];

        if ( argument == "-embedded" )
        {
            if ( ( n + 6 ) < args.count() )
            {
                n++;
                if ( QFile::exists( args[ n ] ) )
                {
                    bool ok_x, ok_y, ok_width, ok_height;

                    setEmbedded(true);
                    m_urls << args[ n ];
                    n++;

                    if ( args[ n ] != "-c" )
                    {
                        printf("Error: expected parameter for -c\r\n");
                        return ( -1 );
                    }

                    n++;
                    m_rcIcon.setX( args[ n ].toInt( &ok_x ) );
                    n++;
                    m_rcIcon.setY( args[ n ].toInt( &ok_y ) );
                    n++;
                    m_rcIcon.setWidth( args[ n ].toInt( &ok_width ) );
                    n++;
                    m_rcIcon.setHeight( args[ n ].toInt( &ok_height ) );

                    emit setEmbeddedState();
                }
                else
                {
                    printf("Error: file %s not exist\r\n", args[ n ].toLocal8Bit().data() );
                    return ( -1 );
                }
            }
            else
            {
                printf("Error: expected parameters for -embedded\r\n");
                return ( -1 );
            }
        }
    }

    if ( !m_isEmbedded )
    {
        m_urls << args;
    }

    return 0;
}

void DeclarativeViewer::init( const QStringList& urls, bool embedded, const QRect& rc )
{
    Q_UNUSED( urls )
    Q_UNUSED( embedded )
    Q_UNUSED( rc )
}

void DeclarativeViewer::onSetGallery( bool isGallery )
{
    m_isGallery = isGallery;
}

QSize DeclarativeViewer::getTextWindowSize(QString url)
{
    QFile f( url );
    if ( !f.open(QIODevice::ReadOnly | QIODevice::Text) )
        return QSize();

    QString text = f.readAll();
    QTextDocument* doc = new QTextDocument( text );
    QSize size =  doc->documentLayout()->documentSize().toSize();
    delete doc;
    size.setWidth( size.width() + 34 );
    size.setHeight( size.height() + 10 );

    QDesktopWidget dw;
    QRect rectDesktop = dw.screenGeometry( this );
    m_width = rectDesktop.width();
    m_height = rectDesktop.height();

    if ( size.width() > m_width * 0.8 )
    {
        size.setWidth( m_width * 0.8 );
    }
    if ( size.height() > m_height * 0.8 )
    {
        size.setHeight( m_height * 0.8 );
    }

    size.setHeight( size.height() + ( m_isEmbedded ? 0 : height_offset ) );
    size.setWidth( ( size.width() < minimumWidth() ) ? minimumWidth() : size.width() ) ;
    size.setHeight( (size.height() < minimumHeight() ) ? minimumHeight() : size.height() );

    return size;
}


void DeclarativeViewer::focusChanged(QWidget *, QWidget *now)
{
    if (m_isEmbedded)
    {
        if (!now)
            close();
    }
}



void DeclarativeViewer::setEmbedded(bool state)
{
    if (state)
    {
        setMinimumSize(50,50);
    }
    else
    {
        setMinimumSize(600,427);
    }
    m_isEmbedded = state;

}

