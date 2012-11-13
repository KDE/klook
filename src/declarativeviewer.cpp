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

#include <KMimeTypeTrader>
#include <KWindowSystem>

#include <plasma/windoweffects.h>
#include <kdeclarative.h>

#include "video.h"
#include "audio.h"
#include "text.h"
#include "file.h"
#include "listitem.h"
#include "previewgenerator.h"
#include "previewprovider.h"
#include "mimeprovider.h"
#include "filemodel.h"
#include "kpartsdeclarativeitem.h"
#include "kpartswidget.h"

static int header_height = 27;
static int border_width = 2;
static int height_offset = 27;
static int arrowIconWidth = 29;
static int arrowIconHeight = 16;

static int min_width = 600;
static int min_height = 427;

DeclarativeViewer::DeclarativeViewer(QWidget* parent)
    : QDeclarativeView(parent)
    , m_lastMousePosition(0, 0)
    , m_isSingleMode(true)
    , m_moving(false)
    , m_resize(false)
    , m_isEmbedded(false)
    , m_isGallery(false)
    , m_currentFile(0)
    , m_region(FRAME_REGION)
{
    KDeclarative kdeclarative;
    kdeclarative.setDeclarativeEngine(engine());
    kdeclarative.initialize();
    kdeclarative.setupBindings();

    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    setMouseTracking(true);

    engine()->addImageProvider("preview", new PreviewProvider);
    engine()->addImageProvider("mime", new MimeProvider);

    setResizeMode(QDeclarativeView::SizeRootObjectToView);

    setMinimumSize(min_width, min_height);

    m_fileModel = new FileModel(this);
    m_fileModel->setRoleNames(ListItem::roleNames());

    PreviewGenerator::instance()->setModel(m_fileModel);

    registerTypes();

    //Remove standard KDE title
    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("background:transparent;");

    connect(qApp, SIGNAL(focusChanged(QWidget*, QWidget*)), SLOT(focusChanged(QWidget*, QWidget*)));
    connect(engine(), SIGNAL(quit()), qApp, SLOT(quit()));
}

DeclarativeViewer::~DeclarativeViewer()
{
    delete PreviewGenerator::instance();
}

void DeclarativeViewer::init(QStringList urls, bool embedded, const QRect& rc, int indexToShow)
{
    if(!urls.size()) {
        qDebug() << "No files to display. Closing...";
        qApp->exit();
        return;
    }

    rootContext()->setContextProperty("indexToShow", indexToShow);

    initModel(urls);

    m_rcIcon = rc;

    setViewMode(urls.count() > 1 ? Multi : Single);
    rootContext()->setContextProperty("embedded", m_isEmbedded);
    setEmbedded(embedded);

    m_currentFile = m_fileModel->file(indexToShow);

    QSize startingSize = m_currentFile->url().isLocalFile()
            ? getPreferredSize(m_currentFile->url().toLocalFile(), File::Progress)
            : QSize();

    KWindowSystem::setState(winId(), NET::SkipTaskbar);
    centerWidget(startingSize.isValid() ? startingSize : QSize(min_width, min_height));
    emit setStartWindow();
}

void DeclarativeViewer::resizeEvent(QResizeEvent *event)
{
    if (KWindowSystem::compositingActive()) {
        //QRegion mask(QRect(QPoint(), size()));
        //Plasma::WindowEffects::enableBlurBehind(winId(), true, mask);
        //Plasma::WindowEffects::overrideShadow(winId(), true);
    }

    QDeclarativeView::resizeEvent(event);
}

void DeclarativeViewer::registerTypes()
{
    qmlRegisterType<Video>("Widgets", 1, 0, "Video");
    qmlRegisterType<Audio>("Widgets", 1, 0, "Audio");
    qmlRegisterType<MyText>("Widgets", 1, 0, "PlainText");
    qmlRegisterType<KPartsDeclarativeItem>("Widgets", 1, 0, "KPart");

    qmlRegisterUncreatableType<File>("Widgets", 1, 0, "File", "This type is registered only for its enums"); // to use File::FileType enum
    rootContext()->setContextProperty("fileModel", m_fileModel);
    rootContext()->setContextProperty("mainWidget",  this);
    rootContext()->setContextProperty("fileName", "");
    rootContext()->setContextProperty("fileUrl", "");
    rootContext()->setContextProperty("fileType", "Undefined");
    rootContext()->setContextProperty("viewMode", "single");
    rootContext()->setContextProperty("embedded", m_isEmbedded);
    rootContext()->setContextProperty("embeddedLayout", "null");
    rootContext()->setContextProperty("previewGenerator", PreviewGenerator::instance());
    rootContext()->setContextProperty("arrowX", .0);
    rootContext()->setContextProperty("arrowY", .0);
}

void DeclarativeViewer::setFullScreen()
{
    if (m_isGallery && !isFullScreen()) {
        showFullScreen();
        emit setFullScreenState();
        return;
    }

    QSize preferredSize;
    File::FileType type = m_currentFile->type();

    if (type != File::Progress) {
        if (m_currentFile->url().isLocalFile()) {
            preferredSize = getPreferredSize(m_currentFile->url().toLocalFile(), type);
        }
        else if(m_currentFile->isLoaded()) {
            // file is remote but it is already downloaded
            preferredSize = getPreferredSize(m_currentFile->tempFilePath(), m_currentFile->type());
        }
    }
    else {
        preferredSize = QSize();
    }

    // this is necessary for following comparisons to geometry
    if(!preferredSize.isValid() || (preferredSize.width() < minimumSize().width())
            || preferredSize.height() < minimumSize().height()){
        preferredSize = minimumSize();
    }

    if (isFullScreen()) {
        showNormal();
        centerWidget(preferredSize);
        return;
    }

    if (geometry().size() == preferredSize) {
        showFullScreen();
        emit setFullScreenState();
    }
    else {
        centerWidget(preferredSize);
    }
}

QSize DeclarativeViewer::getPreferredSize(const QString &path, int type) const
{
    //TODO: use virtual functions for this
    if (type == File::Video) {
        QHash<int, QSize>::const_iterator it = m_videoSizeHints.constFind(m_fileModel->rowFromFile(m_currentFile));
        if(it != m_videoSizeHints.constEnd()) {
            QDesktopWidget dw;
            QSize sz = calculateViewSize(it.value(), dw.screenGeometry(this));

            // + margins values in windowed mode
            sz.setWidth(sz.width() + (m_isEmbedded ? 0 : 6)) ;
            sz.setHeight(sz.height() + (m_isEmbedded ? 0 : height_offset +4));
            return sz;
        }
        return QSize();
    }
    else if (type == File::Image) {
        QImageReader imgReader(path);
        QDesktopWidget dw;
        QSize sz = calculateViewSize(imgReader.size(), dw.screenGeometry(this));
        sz.setWidth(sz.width() + (m_isEmbedded ? 0 : 6)) ;
        sz.setHeight(sz.height() + (m_isEmbedded ? 0 : height_offset +4));
        return sz;
    }
    else if (type == File::Audio ||
             type == File::Directory ||
             type == File::MimetypeFallback) {
        int width = min_width;
        if (type == File::Directory ||
                type == File::MimetypeFallback)
            width += 100;
        return QSize (width, min_height);
    }
    else if (type == File::Txt) {
        return getTextWindowSize(path);
    }
    else if(type == File::OkularFile) {
        return getDocumentWindowSize();
    }
    return QSize();
}

void DeclarativeViewer::initModel(QStringList urls)
{
    // leave only unique entries
    QSet<QString> set;
    for(int i = 0; i < urls.size(); i++) {
        if(set.contains(urls[i])) {
            urls.removeAt(i);
            i--;
        }
        else {
            set.insert(urls[i]);
        }
    }

    m_fileModel->reset();

    QList<ListItem *> items;
    foreach(const QString &str, urls) {
        File *file = new File(KUrl(str));
        items.append(new ListItem(file, m_fileModel));
    }
    m_fileModel->appendRows(items);
}

void DeclarativeViewer::centerWidget(const QSize& sz)
{
    QDesktopWidget dw;
    QRect rectDesktop = dw.screenGeometry(this);
    QSize sz1 = sz;

    if (m_isEmbedded) {
        int iconOffset = 5;
        int desktopMargin = 70;

        QRect top(rectDesktop.x() + desktopMargin,
                  rectDesktop.y() + desktopMargin ,
                  rectDesktop.width() - 2 * desktopMargin ,
                  m_rcIcon.y() - desktopMargin - iconOffset);

        QRect left(rectDesktop.x() + desktopMargin,
                   rectDesktop.y() + desktopMargin,
                   m_rcIcon.x() - iconOffset - rectDesktop.x() - desktopMargin,
                   rectDesktop.height()- 2 * desktopMargin);

        QRect right(m_rcIcon.topRight().x(),
                    rectDesktop.y() + desktopMargin,
                    rectDesktop.width() - m_rcIcon.topRight().x() - desktopMargin - iconOffset,
                    rectDesktop.height() - 2 * desktopMargin);

        int topArea = top.width()*top.height();
        int leftArea = left.width()*left.height();
        int rightArea = right.width()*right.height();

        if ((topArea > leftArea) && (topArea > rightArea)) {
            sz1.setHeight(sz1.height() + arrowIconHeight);
            sz1 = inscribedRectToRect(sz1, top.size());
            int x = m_rcIcon.x() + m_rcIcon.width()/2 - sz1.width()/2;
            int y = m_rcIcon.y() - iconOffset - sz1.height();
            x = qMax(x , desktopMargin);
            x = qMin(x, top.topRight().x() - sz1.width());
            QRect rect(x,y,sz1.width()+2,sz1.height()+2); // +2 - border width in embedded mode
            rootContext()->setContextProperty("embeddedLayout", "top");
            //Set arrow icon coordinates
            m_rcArrow.setX(m_rcIcon.x() + m_rcIcon.width()/2 - arrowIconWidth/2 -rect.x());
            m_rcArrow.setY(rect.height() - arrowIconHeight);
            m_rcArrow.setWidth(arrowIconWidth);
            m_rcArrow.setHeight(arrowIconHeight);
            m_posArrow = BOTTOM;
            setGeometry(rect);
        }
        else if (leftArea > rightArea) {
            sz1.setWidth(sz1.width()+ arrowIconHeight);
            sz1 = inscribedRectToRect(sz1, left.size());
            int x = m_rcIcon.x() - sz1.width() - iconOffset;
            int y = m_rcIcon.y() + m_rcIcon.height()/2 - sz1.height()/2;
            y = qMax(y , desktopMargin);
            y = qMin(y, left.bottomLeft().y() -sz1.height());
            QRect rect(x,y,sz1.width()+2,sz1.height()+2);
            rootContext()->setContextProperty("embeddedLayout", "left");
            //Set arrow icon coordinates
            m_rcArrow.setX(rect.width() - arrowIconHeight);
            m_rcArrow.setY(m_rcIcon.y() + m_rcIcon.height()/2 - arrowIconWidth/2 - rect.y());
            m_rcArrow.setWidth(arrowIconHeight);
            m_rcArrow.setHeight(arrowIconWidth);
            m_posArrow = RIGHT;
            setGeometry(rect);
        }
        else {
            sz1.setWidth(sz1.width()+ arrowIconHeight);
            sz1 = inscribedRectToRect(sz1, right.size());
            int x = m_rcIcon.topRight().x() + iconOffset;
            int y = m_rcIcon.y() + m_rcIcon.height()/2 - sz1.height()/2;
            y = qMax(y , desktopMargin);
            y = qMin(y, right.bottomLeft().y() - sz1.height());
            QRect rect(x,y,sz1.width()+2,sz1.height()+2);
            rootContext()->setContextProperty("embeddedLayout", "right");
            //Set arrow icon coordinates
            m_rcArrow.setX(0);
            m_rcArrow.setY(m_rcIcon.y() + m_rcIcon.height()/2 - arrowIconWidth/2 - rect.y());
            m_rcArrow.setWidth(arrowIconHeight);
            m_rcArrow.setHeight(arrowIconWidth);
            m_posArrow = LEFT;
            setGeometry(rect);
        }
        rootContext()->setContextProperty("arrowX", m_rcArrow.x());
        rootContext()->setContextProperty("arrowY", m_rcArrow.y());
        emit setEmbeddedState();
    }
    else {
        int w = sz.width();
        int h = sz.height();
        QRect rect((rectDesktop.width() - w) / 2,
                    (rectDesktop.height() - h) / 2,
                    w , h);
        rect.moveTop(rect.y() - height_offset / 2);
        setGeometry(rect);
        //move(QApplication::desktop()->screen()->rect().center() - rect().center());
    }

    if (!isVisible()) {
        show();
    }
    activateWindow();
}

void DeclarativeViewer::updateCurrentFile(int index)
{
    if(index == -1) {
        return;
    }
    m_currentFile = m_fileModel->file(index);
}

QString DeclarativeViewer::serviceForFile(int index) const
{
    const File *file = m_fileModel->file(index);
    if (!file->mime().isEmpty()) {
        KService::Ptr ptr = KMimeTypeTrader::self()->preferredService(file->mime());
        KService *serv = ptr.data();
        if(!ptr.isNull()) {
            return serv->name();
        }
    }
    return QString();
}

void DeclarativeViewer::resizeToPreferredSize(int index)
{
    File *file = m_fileModel->file(index);
    const QString path = file->url().isLocalFile() ? file->url().toLocalFile() : file->tempFilePath();
    QSize preferredSize = getPreferredSize(path, file->type());

    // this is necessary for following comparisons to geometry
    if (!preferredSize.isValid() || (preferredSize.width() < minimumSize().width()) ||
            preferredSize.height() < minimumSize().height()) {
        preferredSize = minimumSize();
    }

    if (preferredSize != geometry().size()) {
        centerWidget(preferredSize);
    }
}

void DeclarativeViewer::setVideoSizeHint(int width, int height, int index)
{
    m_videoSizeHints.insert(index, QSize(width, height));
}

WidgetRegion DeclarativeViewer::calculateWindowRegion(const QPoint& mousePos)
{
    QPointF pos;
    pos = mousePos;
    if (m_isEmbedded) {
        QRegion region;
        QRect r = rect();
        if (m_posArrow == BOTTOM) {
            QRect r1(r.bottomLeft().x(), r.bottomLeft().y()-arrowIconHeight, r.width(),arrowIconHeight);
            region.setRects(&r1,1);
            region = region.subtracted(QRegion(m_rcArrow));
        }
        else if (m_posArrow == LEFT) {
            QRect r2(r.x(), r.y(), m_rcArrow.width(), r.height());
            region.setRects(&r2,1);
            region = region.subtracted(QRegion(m_rcArrow));
        }
        else if (m_posArrow == RIGHT) {
            QRect r3(r.topRight().x() - m_rcArrow.width(), r.y(), m_rcArrow.width(), r.height());
            region.setRects(&r3,1);
            region = region.subtracted(QRegion(m_rcArrow));
        }
        if (region.contains(pos.toPoint())) {
            return ARROW_NULL_REGION;
        }
        return FRAME_REGION;
    }


    QRectF r = rect();
    QRectF headerTitle;
    QRectF header1(6 + 42 * 2, border_width + 1, 60, header_height - border_width + 1);
    QRectF header2(r.width() - 42 * 2 - 12 - 6, border_width + 1, 12, header_height- border_width + 1);
    QRegion headerR;

    if (m_isSingleMode) {
        headerTitle.setRect(border_width + 1,
                             border_width + 1,
                             r.width() - border_width - 270,
                             header_height - border_width + 1);
        headerR = headerR.united(headerTitle.toRect());
        headerR = headerR.united(header2.toRect());
    }
    else {
        headerTitle.setRect(6 + 42 * 3 + 12,
                             border_width + 1,
                             r.width() - 414,
                             header_height - border_width + 1);
        headerR = headerR.united(headerTitle.toRect());
        headerR = headerR.united(header1.toRect());
        headerR = headerR.united(header2.toRect());
    }

    QRectF topBorder(0, 0, r.width(), border_width);
    QRectF leftBorder(0, 0, border_width, r.height());
    QRectF rightBorder(r.width()-border_width, 0, border_width, r.height());
    QRectF bottomBorder(0, r.height()-border_width, r.width(), border_width);

    if (headerR.contains(pos.toPoint())) {
        return HEADER_REGION;
    }
    else if (topBorder.contains(pos)) {
        if (leftBorder.contains(pos)) {
            return TOP_LEFT_CORNER_REGION;
        }
        else if (rightBorder.contains(pos)) {
            return TOP_RIGHT_CORNER_REGION;
        }
        else {
            return TOP_BORDER_REGION;
        }
    }
    else if (bottomBorder.contains(pos)) {
        if (leftBorder.contains(pos)) {
            return BOTTOM_LEFT_CORNER_REGION;
        }
        else if (rightBorder.contains(pos)) {
            return BOTTOM_RIGHT_CORNER_REGION;
        }
        else {
            return BOTTOM_BORDER_REGION;
        }
    }
    else if (leftBorder.contains(pos)) {
        return LEFT_BORDER_REGION;
    }
    else if (rightBorder.contains(pos)) {
        return RIGHT_BORDER_REGION;
    }

    return FRAME_REGION;
}

void DeclarativeViewer::mousePressEvent(QMouseEvent* event)
{
    if (!m_isEmbedded) {
        if (!isFullScreen()) {
            m_region = calculateWindowRegion(event->pos());
            if (event->button() == Qt::LeftButton) {
                if (m_region == HEADER_REGION) {
                    m_moving = true;
                    m_lastMousePosition = event->globalPos();
                    setCursor(QCursor(Qt::SizeAllCursor));
                }
                else if (m_region != FRAME_REGION) {
                    m_resize = true;
                    m_lastMousePosition = event->globalPos();
                }
            }
            event->accept();
        }
    }
    else {
        if (calculateWindowRegion(event->pos()) == ARROW_NULL_REGION) {
            close();
        }
    }

    QDeclarativeView::mousePressEvent(event);
}

void DeclarativeViewer::mouseMoveEvent(QMouseEvent* event)
{
    QDeclarativeView::mouseMoveEvent(event);
    if (isFullScreen() || m_isEmbedded) {
        return;
    }

    if (event->buttons().testFlag(Qt::LeftButton) && m_moving) {
        if (viewport()->cursor().shape() != (Qt::SizeAllCursor))
            viewport()->setCursor(QCursor(Qt::SizeAllCursor));
        window()->move(window()->pos() + (event->globalPos() - m_lastMousePosition));
        m_lastMousePosition = event->globalPos();
    }
    else if (event->buttons().testFlag(Qt::LeftButton) && m_resize) {
        QPoint p = mapToGlobal(event->pos()) - geometry().topLeft();
        QPoint offset = event->globalPos() - m_lastMousePosition;
        switch (m_region) {
        case LEFT_BORDER_REGION:
            if (offset.x() * (-1) + width() > minimumWidth()) {
                window()->move((window()->pos() + offset).x(), y());
                resize(offset.x() * (-1) + width(), height());
                m_lastMousePosition = event->globalPos();
            }
            break;
        case TOP_BORDER_REGION:
            if (offset.y()*(-1) + height() > minimumHeight()) {
                window()->move(x(), (window()->pos() + offset).y());
                resize(width(), offset.y() * (-1) + height());
                m_lastMousePosition = event->globalPos();
            }
            break;
        case TOP_LEFT_CORNER_REGION:
            if (offset.x() * (-1) + width() > minimumWidth()) {
                window()->move((window()->pos() + offset).x(), y());
                resize(offset.x() * (-1) + width(), height());
                m_lastMousePosition.setX(event->globalPos().x());
            }
            if (offset.y() * (-1) + height() > minimumHeight()) {
                window()->move(x(), (window()->pos() + offset).y());
                resize(width(), offset.y() * (-1) + height());
                m_lastMousePosition.setY(event->globalPos().y());
            }
            break;
        case TOP_RIGHT_CORNER_REGION:
            resize(p.x(), height());
            if (offset.y() * (-1) + height() > minimumHeight()) {
                window()->move(x(), (window()->pos() + offset).y());
                resize(width(), offset.y() * (-1) + height());
                m_lastMousePosition.setY(event->globalPos().y());
            }
            break;
        case BOTTOM_LEFT_CORNER_REGION:
            resize(width() , p.y());
            if (offset.x() * (-1) + width() > minimumWidth()) {
                window()->move((window()->pos() + offset).x(), y());
                resize(offset.x() * (-1) + width(), height());
                m_lastMousePosition.setX(event->globalPos().x());
            }
            break;
        case RIGHT_BORDER_REGION:
            resize(p.x(), height());
            break;
        case BOTTOM_BORDER_REGION:
            resize(width() , p.y());
            break;
        default:
            resize(p.x(), p.y());
        }
    }
    else {
        m_region = calculateWindowRegion(event->pos());
        if ((m_region == TOP_BORDER_REGION) || (m_region == BOTTOM_BORDER_REGION)) {
            if (viewport()->cursor().shape() != (Qt::SizeVerCursor)) {
                viewport()->setCursor(QCursor(Qt::SizeVerCursor));
            }
        }
        else if ((m_region == LEFT_BORDER_REGION) || (m_region == RIGHT_BORDER_REGION)) {
            if (viewport()->cursor().shape() != (Qt::SizeHorCursor)) {
                viewport()->setCursor(QCursor(Qt::SizeHorCursor));
            }
        }
        else if ((m_region == TOP_LEFT_CORNER_REGION) || (m_region == BOTTOM_RIGHT_CORNER_REGION)) {
            if (viewport()->cursor().shape() != (Qt::SizeFDiagCursor)) {
                viewport()->setCursor(QCursor(Qt::SizeFDiagCursor));
            }
        }
        else if ((m_region == TOP_RIGHT_CORNER_REGION) || (m_region == BOTTOM_LEFT_CORNER_REGION)) {
            if (viewport()->cursor().shape() != (Qt::SizeBDiagCursor)) {
                viewport()->setCursor(QCursor(Qt::SizeBDiagCursor));
            }
        }
        else if (viewport()->cursor().shape() != (Qt::ArrowCursor)) {
            viewport()->setCursor(QCursor(Qt::ArrowCursor));
        }
    }
}

void DeclarativeViewer::mouseReleaseEvent(QMouseEvent* event)
{
    if (!isFullScreen()) {
        if (m_moving) {
            m_moving = false;
            viewport()->setCursor(QCursor(Qt::ArrowCursor));
        }

        if (m_resize) {
            m_resize = false;
            viewport()->setCursor(QCursor(Qt::ArrowCursor));
        }
        event->accept();
    }
    QDeclarativeView::mouseReleaseEvent(event);
}

void DeclarativeViewer::setViewMode(DeclarativeViewer::ViewMode mode)
{
    m_isSingleMode = (mode == Single);
    rootContext()->setContextProperty("viewMode", ((mode == Single) ? "single" : "multi"));
}

void DeclarativeViewer::onSetGallery(bool isGallery)
{
    m_isGallery = isGallery;
}

QSize DeclarativeViewer::getTextWindowSize(QString url) const
{
    QFile f(url);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QSize();
    }

    QString text = f.readAll();
    QTextDocument* doc = new QTextDocument(text);
    QSize size =  doc->documentLayout()->documentSize().toSize();
    delete doc;

    size.setWidth(size.width() + 34);
    size.setHeight(size.height() + 10);

    QDesktopWidget dw;
    QSize desktopSize = dw.screenGeometry(this).size();

    if (size.width() > desktopSize.width() * 0.8) {
        size.setWidth(desktopSize.width() * 0.8);
    }
    if (size.height() > desktopSize.height() * 0.8) {
        size.setHeight(desktopSize.height() * 0.8);
    }

    size.setHeight(size.height() + (m_isEmbedded ? 0 : height_offset));

    size.setWidth(qMax(size.width(), minimumWidth()));
    size.setHeight(qMax(size.height(), minimumHeight()));

    return size;
}

void DeclarativeViewer::focusChanged(QWidget*, QWidget* now)
{
    if (m_isEmbedded && !now) {
        close();
    }
}

void DeclarativeViewer::setEmbedded(bool state)
{
    if (state) {
        setWindowFlags(windowFlags() | Qt::ToolTip);
    }
    QSize minSize = state ? QSize(50, 50) : QSize(min_width, min_height);
    setMinimumSize(minSize);
    m_isEmbedded = state;
}

QSize calculateViewSize(const QSize& sz, const QRect &desktop)
{
    QSize szItem = sz;
    int wDesktop = desktop.width() * 8 / 10;
    int hDesktop = desktop.height() * 8 / 10;

    szItem = inscribedRectToRect(sz, QSize(wDesktop, hDesktop));
    return szItem;
}

// function inscribing rect sz1 into rect sz2
QSize inscribedRectToRect(const QSize& sz1, const QSize& sz2)
{
    QSize sz = sz1;
    if ((sz.height() > sz2.height()) || (sz.width() > sz2.width())) {
        sz.scale(sz2, Qt::KeepAspectRatio);
    }
    return sz;
}

