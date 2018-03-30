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


#ifndef AUDIO_H
#define AUDIO_H

#include <QtCore/QTime>
#include <QtQuick/QQuickItem>
#include <KDE/Phonon/MediaObject>
#include <KDE/Phonon/AudioOutput>

#include <KUrl>

class QHBoxLayout;

class Audio : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(bool playing READ playing)
    Q_PROPERTY(bool paused READ paused)
    Q_PROPERTY(qlonglong totalTime READ totalTime NOTIFY totalTimeChanged)
    Q_PROPERTY(QTime duration READ duration)
    Q_PROPERTY(qlonglong position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(bool preview READ isPreview WRITE setPreview)
    Q_PROPERTY(bool ready READ isReady WRITE setReady NOTIFY ready)

    Q_PROPERTY(QString artist READ artist NOTIFY artistChanged)
    Q_PROPERTY(QString album READ album NOTIFY albumChanged)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString composer READ composer  NOTIFY composerChanged)
    Q_PROPERTY(QString genre READ genre  NOTIFY genreChanged)

public:
    Audio(QQuickItem* parent = 0);
    ~Audio();

public slots:
    QString source() const;
    void setSource(const KUrl &source);

    bool playing() const;
    bool paused() const;
    void setPause();

    bool isReady();
    void setReady(bool b);

    qlonglong totalTime() const;
    QTime duration();

    qlonglong position() const;
    void setPosition(qlonglong pos);

    QString artist();
    QString album();
    QString title();
    QString composer();
    QString genre();

    void play_or_pause();
    void play();
    void pause();

    void onTotalTimeChanged(qint64);
    void onTicked(qint64);

    void onFinished();

    void stateChanged(Phonon::State oldState, Phonon::State newState);

    bool isPreview() const;
    void setPreview(bool preview);

protected:

Q_SIGNALS:
    void clicked(bool);
    void sourceChanged();
    void totalTimeChanged();
    void positionChanged();
    void ticked(QVariant tick);
    void playFinished();
    void ready();

    void artistChanged();
    void albumChanged();
    void titleChanged();
    void composerChanged();
    void genreChanged();

private:
    QString guessEncoding(const QByteArray &data);

    Phonon::MediaObject* m_mediaObject;

    bool m_isPreview;
    bool m_isReady;
};

#endif // AUDIO_H
