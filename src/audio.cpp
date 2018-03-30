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

#include "audio.h"

#include <KEncodingProber>
#include <QtCore/QFileInfo>
#include <QtCore/QString>
#include <QtCore/QTextCodec>

#include <KDE/Phonon/AudioOutput>
#include <KDE/Phonon/MediaObject>
#include <KDE/Phonon/VideoWidget>

Audio::Audio(QQuickItem* parent)
    : QQuickItem(parent)
    , m_isPreview(false)
    , m_isReady(false)
{
    m_mediaObject = Phonon::createPlayer(Phonon::MusicCategory);
    m_mediaObject->setTickInterval(1000);

    QObject::connect(m_mediaObject, SIGNAL(tick(qint64)), SLOT(onTicked(qint64)));
    QObject::connect(m_mediaObject, SIGNAL(totalTimeChanged(qint64)), SLOT(onTotalTimeChanged(qint64)));
    QObject::connect(m_mediaObject, SIGNAL(finished()), SLOT(onFinished()));
    QObject::connect(m_mediaObject, SIGNAL(stateChanged(Phonon::State, Phonon::State)),
                     SLOT(stateChanged(Phonon::State, Phonon::State)));
}

Audio::~Audio()
{
    delete m_mediaObject;
}

void Audio::setPause()
{
    if (!isVisible()) {
        pause();
    }
    else {
        play();
    }
}

bool Audio::isReady()
{
    return m_isReady;
}

void Audio::setReady(bool b)
{
    m_isReady = b;
    emit ready();
}

void Audio::onFinished()
{
    emit playFinished();
    setPosition(0);
}

QString Audio::source() const
{
    return m_mediaObject->currentSource().url().toString();
}

void Audio::setSource(const KUrl& source)
{
    m_mediaObject->setCurrentSource(source);
    emit sourceChanged();
}

qlonglong Audio::position() const
{
    return m_mediaObject->currentTime();
}

void Audio::setPosition(qlonglong pos)
{
    m_mediaObject->seek(pos);
    emit positionChanged();
}

qlonglong Audio::totalTime() const
{
    return m_mediaObject->totalTime();
}

QTime Audio::duration()
{
    QTime t;
    t = t.addMSecs(m_mediaObject->totalTime());
    return t;
}

void Audio::onTotalTimeChanged(qint64 t)
{
    Q_UNUSED(t)
    emit totalTimeChanged();
}

void Audio::onTicked(qint64 t)
{
    emit ticked(QVariant(t));
}

bool Audio::playing() const
{
    return (m_mediaObject->state() == Phonon::PlayingState);
}

bool Audio::paused() const
{
    return (m_mediaObject->state() == Phonon::PausedState);
}

void Audio::play()
{
    m_mediaObject->play();
}

void Audio::pause()
{
    m_mediaObject->pause();
}

void Audio::play_or_pause()
{
    if (playing()) {
        pause();
    }
    else {
        play();
    }
}

bool Audio::isPreview() const
{
    return m_isPreview;
}

void Audio::setPreview(bool preview)
{
    m_isPreview = preview;
}

void Audio::stateChanged(Phonon::State newState, Phonon::State oldState)
{
    Q_UNUSED(newState)
    if ((oldState == Phonon::LoadingState) || (oldState == Phonon::StoppedState)) {
        setReady(true);
    }
}

QString Audio::artist()
{
    QString artists;
    QStringList listArtists = m_mediaObject->metaData("ARTIST");
    if (!listArtists.empty()) {
        artists = guessEncoding(listArtists.join(", ").toAscii());
    }
    return artists;
}

QString Audio::album()
{
    QString albums;
    QStringList listAlbums = m_mediaObject->metaData("ALBUM");
    if (!listAlbums.empty()) {
        albums = guessEncoding(listAlbums.join(", ").toAscii());
    }
    return albums;
}

QString Audio::title()
{
    QString titles;
    QStringList listTitles = m_mediaObject->metaData("TITLE");

    titles = guessEncoding(listTitles.join(", ").toAscii());
    if (titles.isEmpty()) {
        QFileInfo fi(m_mediaObject->currentSource().url().path());
        titles = fi.fileName();
    }

    return titles;
}

QString Audio::guessEncoding(const QByteArray &data)
{
    KEncodingProber prober(KEncodingProber::Universal);
    prober.feed(data);
    QString str = prober.confidence() > 0.7
            ? QTextCodec::codecForName(prober.encoding())->toUnicode(data)
            : QString::fromUtf8(data);

    return str;
}

QString Audio::composer()
{
    QString composers;
    QStringList listcomposers = m_mediaObject->metaData("COMPOSER");
    if (!listcomposers.empty()) {
        composers = guessEncoding(listcomposers.join(", ").toAscii());
    }

    return composers.toUtf8();
}

QString Audio::genre()
{
    QString genres;
    QStringList listgenres = m_mediaObject->metaData("GENRE");
    if (!listgenres.empty()) {
        genres = guessEncoding(listgenres.join(", ").toAscii());
    }

    return genres;
}

