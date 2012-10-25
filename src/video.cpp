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
#include <QHBoxLayout>

#include <KDE/Phonon/MediaObject>
#include <KDE/Phonon/VideoWidget>
#include <KDE/Phonon/AudioOutput>

Video::Video(QGraphicsItem* parent)
    : QGraphicsProxyWidget(parent)
    , m_layout(0)
    , m_videoWidth(640)
    , m_videoHeight(480)
    , m_isPreview(false)
    , m_isReady(false)
    , m_player(0)
{
    m_wid = new QWidget();

    m_player = new Phonon::VideoPlayer(Phonon::VideoCategory, m_wid);
    m_player->setAttribute(Qt::WA_NoSystemBackground);
    m_player->mediaObject()->setTickInterval( 1000 );

    m_layout = new QHBoxLayout();
    m_layout->setMargin(0);
    m_layout->addWidget(m_player);
    m_wid->setLayout(m_layout);
    setWidget(m_wid);

    QObject::connect(m_player->mediaObject(), SIGNAL(tick(qint64)), SLOT(onTicked(qint64)));
    QObject::connect(m_player->mediaObject(), SIGNAL(totalTimeChanged(qint64)), SLOT(onTotalTimeChanged(qint64)));
    QObject::connect(m_player->mediaObject(), SIGNAL(finished()), SLOT(onFinished()));
    QObject::connect(m_player->mediaObject(), SIGNAL(metaDataChanged()), SLOT(onMetaDataChanged()));
    QObject::connect(m_player->mediaObject(), SIGNAL(stateChanged(Phonon::State, Phonon::State)),
                      SLOT(stateChanged(Phonon::State,Phonon::State)));
}

Video::~Video()
{

}

void Video::setPause()
{
    if (!isVisible())
        pause();
    else
        play();
}

bool Video::isReady()
{
    return m_isReady;
}

void Video::setReady(bool b)
{
    m_isReady = b;
    emit ready();
}

void Video::onFinished()
{
    emit playFinished();
    setPosition(0);
}

bool Video::muted() const
{
    return m_player->audioOutput()->isMuted();
}

void Video::setMuted(bool m)
{
    // !!!! Doesnt't work in phonon !!!!!
    m_player->audioOutput()->setMuted( m );
}

QString Video::source() const
{
    return m_player->mediaObject()->currentSource().url().toString();
}

void Video::setSource(const KUrl &source)
{
    m_player->mediaObject()->setCurrentSource(source);
    emit sourceChanged();
}

void Video::onMetaDataChanged()
{
    m_videoWidth = m_player->videoWidget()->sizeHint().width();
    m_videoHeight = m_player->videoWidget()->sizeHint().height();

    QObject::disconnect(m_player->mediaObject(), SIGNAL(metaDataChanged()), this, SLOT(onMetaDataChanged()));
}

qlonglong Video::position() const
{
    return m_player->mediaObject()->currentTime();
}

void Video::setPosition( qlonglong pos )
{
    m_player->mediaObject()->seek( pos );
    emit positionChanged();
}

qlonglong Video::totalTime() const
{
    return m_player->mediaObject()->totalTime();
}

void Video::onTotalTimeChanged( qint64 t )
{
    Q_UNUSED( t )
    emit totalTimeChanged();
}

void Video::onTicked( qint64 t )
{
    emit ticked( QVariant( t ) );
}

bool Video::playing() const
{
    return m_player->isPlaying();
}

bool Video::paused() const
{
    return m_player->isPaused();
}

void Video::play()
{
    m_player->play();
}

void Video::pause()
{
    m_player->pause();
}

void Video::play_or_pause()
{
    if ( m_player->isPlaying() )
        m_player->pause();
    else
        m_player->play();
}

bool Video::isPreview() const
{
    return m_isPreview;
}

void Video::setPreview( bool preview )
{
    m_isPreview = preview;
}

void Video::stateChanged( Phonon::State newState, Phonon::State oldState )
{
    Q_UNUSED( newState )
    if ((oldState == Phonon::LoadingState) || (oldState == Phonon::StoppedState))
        setReady( true );
}
