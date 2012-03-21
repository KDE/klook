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


#include "video.h"

#include <QPainter>
#include <QDebug>
#include <QMultiMap>
#include <QList>
#include <QHBoxLayout>

#include <KDE/Phonon/MediaObject>
#include <KDE/Phonon/VideoWidget>
#include <KDE/Phonon/AudioOutput>

//#include <kfilemetainfo.h>
//#include <kfilemetainfoitem.h>
//#include <predicateproperties.h>



Phonon::VideoPlayer* MyVideo::m_player = 0;

MyVideo::MyVideo( QGraphicsItem* parent )
    : QGraphicsProxyWidget( parent )
    /*, m_player( 0 )*/
    , m_videoWidth( 640 )
    , m_videoHeight( 480 )
    , m_isPreview( false )
    , m_isReady(false)
    , m_layout(0)
{
    /*
    qDebug() << "count is " << count;
    count++;

    */
    //qDebug()<< "++++++++New copy of MyVideo++++++" << this;

    //m_wid->setAttribute(Qt::WA_TranslucentBackground);
    //qDebug()<< "new m_wid" << m_wid ;
    //m_ptr = m_wid;
    m_wid = new QWidget();
    if (!m_player)
    {
        //qDebug("create new m_player");
        m_player = new Phonon::VideoPlayer( Phonon::VideoCategory,m_wid);
        m_player->setAttribute( Qt::WA_NoSystemBackground );
        m_player->mediaObject()->setTickInterval( 1000 );
    }


   /* else
    {
      //  if (count < 2)

    }
    */
    //qDebug() << m_player;

    m_layout = new QHBoxLayout();
    m_layout->setMargin(0);
    m_wid->setLayout(m_layout);
    setWidget( m_wid );
    //qDebug()<< m_wid;

    QObject::connect( m_player->mediaObject(), SIGNAL( tick( qint64 ) ), SLOT( onTicked( qint64 ) ) );
    QObject::connect( m_player->mediaObject(), SIGNAL( totalTimeChanged( qint64 ) ), SLOT( onTotalTimeChanged( qint64 ) ) );
    QObject::connect( m_player->mediaObject(), SIGNAL( finished() ), SLOT( onFinished() ) );
    QObject::connect( m_player->mediaObject(), SIGNAL( metaDataChanged() ), SLOT( onMetaDataChanged() ) );
    QObject::connect( m_player->mediaObject(), SIGNAL( stateChanged( Phonon::State, Phonon::State ) ),
                      SLOT( stateChanged( Phonon::State,Phonon::State ) ) );
}

MyVideo::~MyVideo()
{
    //qDebug() << "------------Delete MyVideo--------------" << this;
    /*
    qDebug() << "count is " << count;
    //m_player->setParent();

    qDebug() <<  m_player->mediaObject()->currentSource().url().toString();
    qDebug() <<  m_wid;
    */
    QObjectList list = m_wid->children();
    //qDebug() << "m_wid->children() = " <<list.size();
    if (list.size() > 1)
    {
        //qDebug() << "==============Remove parent=============" <<list.size();
        m_player->stop();
        m_player->setParent(0);
    }
/*


    //if (count < 2)
        //m_player->setParent(0);
    //delete m_wid;

    if (m_player->isEnabled())
        qDebug("enabled");
        */
    //qDebug("delete video widget");
    //m_player->deleteLater();
    //count--;
    //qDebug() << "   ";
}


void MyVideo::setPlayerParent()
{
    //qDebug() << "=============setParent===========";
//      /qDebug()<< m_wid;
    m_layout->addWidget(m_player);
    //m_player->setParent(m_layout);
}

void MyVideo::setPause()
{
    if ( !isVisible() )
        pause();
    else
        play();
}

bool MyVideo::isReady()
{
    return m_isReady;
}

void MyVideo::setReady(bool b)
{
    m_isReady = b;
    emit ready();
}

void MyVideo::onFinished()
{
    emit playFinished();
    setPosition( 0 );
}

bool MyVideo::muted() const
{
    return m_player->audioOutput()->isMuted();
}

void MyVideo::setMuted( bool m )
{
    // !!!! Doesnt't work in phonon !!!!!
    //qDebug() << "  MyVideo::setMuted(): mute is " << m;
    m_player->audioOutput()->setMuted( m );
}

QString MyVideo::source() const
{
    return m_player->mediaObject()->currentSource().url().toString();
}

void MyVideo::setSource( const QString& source )
{
    //qDebug()<< "=============Set Source===========" << source << this;
    QUrl url( source );

    //if ( url != m_player->mediaObject()->currentSource().url() )
    //{
        m_player->mediaObject()->setCurrentSource( url );
        emit sourceChanged();
    //}

    //    play();

    /*
    KFileMetaInfo* mi = new KFileMetaInfo(source);//, QString(), KFileMetaInfo::TechnicalInfo);

    qDebug() << "file meta info: ";
    QStringList keys = mi->keys();

    for (int i=0; i< keys.count(); i++)
    {
        if (keys[i].contains("height") )
        {
            KFileMetaInfoItem mii = mi->item(keys[i]);
            qDebug() << " meta info item name is  " << mii.name() << " ( " << mii.value().toInt() << " )";

            PredicateProperties pp = mii.properties();
            qDebug() << " properties attributes is  " << pp.attributes();


        }

        if (keys[i].contains("width") )
        {
            KFileMetaInfoItem mii = mi->item(keys[i]);
            qDebug() << " meta info item name is  " << mii.name() << " ( " << mii.value().toInt() << " )";

            PredicateProperties pp = mii.properties();
            qDebug() << " properties attributes is  " << pp.attributes();
        }
    }

    delete mi;
    */
}

void MyVideo::onMetaDataChanged()
{
    m_videoWidth = m_player->videoWidget()->sizeHint().width();
    m_videoHeight = m_player->videoWidget()->sizeHint().height();
    //emit ready();

    QObject::disconnect( m_player->mediaObject(), SIGNAL( metaDataChanged() ), this, SLOT( onMetaDataChanged() ) );
}

qlonglong MyVideo::position() const
{
    return m_player->mediaObject()->currentTime();
}

void MyVideo::setPosition( qlonglong pos )
{
    m_player->mediaObject()->seek( pos );
    emit positionChanged();
}

qlonglong MyVideo::totalTime() const
{
    return m_player->mediaObject()->totalTime();
}

void MyVideo::onTotalTimeChanged( qint64 t )
{
    Q_UNUSED( t )
    emit totalTimeChanged();
}

void MyVideo::onTicked( qint64 t )
{
    emit ticked( QVariant( t ) );
}

bool MyVideo::playing() const
{
    return m_player->isPlaying();
}

bool MyVideo::paused() const
{
    return m_player->isPaused();
}

void MyVideo::play()
{
    m_player->play();
}

void MyVideo::pause()
{
    m_player->pause();
}

void MyVideo::play_or_pause()
{
    if ( m_player->isPlaying() )
        m_player->pause();
    else
        m_player->play();
}

void MyVideo::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
    // It is added for deciding Qt bug when while viewing video colors are invereted
    // When using Phonon with the GStreamer backed on linux inside QGraphicsView, the colors are wrong, it is like being in BGR instead of RGB.
    // It's Qt bug (https://bugreports.qt.nokia.com//browse/QTBUG-8737).
    QPainter* painter_inverted = new QPainter();

    QImage invertedColor( boundingRect().width(), boundingRect().height(), QImage::Format_RGB32 );

    painter_inverted->begin( &invertedColor );

    QGraphicsProxyWidget::paint( painter_inverted, option,widget );

    painter_inverted->end();
    painter->drawImage( 0, 0, invertedColor.rgbSwapped() );
    delete painter_inverted;
}

bool MyVideo::isPreview() const
{
    return m_isPreview;
}

void MyVideo::setPreview( bool preview )
{
    m_isPreview = preview;
}

void MyVideo::stateChanged( Phonon::State newState, Phonon::State oldState )
{
    Q_UNUSED( newState )
    if ( oldState == Phonon::LoadingState )// && newState != Phonon::ErrorState)
    {
        setReady(true);
    }
}
