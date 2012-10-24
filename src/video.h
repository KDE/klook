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


#ifndef VIDEO_H
#define VIDEO_H

#include <QGraphicsProxyWidget>
#include <KDE/Phonon/VideoPlayer>

class QHBoxLayout;

class Video : public QGraphicsProxyWidget
{
    Q_OBJECT
    Q_PROPERTY( QString source READ source WRITE setSource NOTIFY sourceChanged )
    Q_PROPERTY( bool playing READ playing )
    Q_PROPERTY( bool paused READ paused )
    Q_PROPERTY( bool muted READ muted WRITE setMuted )
    Q_PROPERTY( qlonglong totalTime READ totalTime NOTIFY totalTimeChanged )
    Q_PROPERTY( qlonglong position READ position WRITE setPosition NOTIFY positionChanged )
    Q_PROPERTY( int videoWidth READ videoWidth )
    Q_PROPERTY( int videoHeight READ videoHeight )
    Q_PROPERTY( bool preview READ isPreview WRITE setPreview )
    Q_PROPERTY( bool ready READ isReady WRITE setReady NOTIFY ready )

public:
    Video(QGraphicsItem* parent = 0);
    ~Video();

public slots:
    QString source() const;
    void setSource( const QString& source );

    bool playing() const;
    bool paused() const;
    void setPause();

    bool isReady();
    void setReady(bool b);

    bool muted() const;
    void setMuted( bool m );

    int videoWidth() const { return m_videoWidth; }
    int videoHeight() const { return m_videoHeight; }

    qlonglong totalTime() const;

    qlonglong position() const;
    void setPosition( qlonglong pos );

    void play_or_pause();
    void play();
    void pause();

    void onTotalTimeChanged( qint64 );
    void onTicked( qint64 );

    void onFinished();

    void onMetaDataChanged();
    void stateChanged(Phonon::State oldState, Phonon::State newState);

    bool isPreview() const;
    void setPreview( bool preview );
    void setPlayerParent();

Q_SIGNALS:
    void clicked( bool );
    void sourceChanged();
    void totalTimeChanged();
    void positionChanged();
    void ticked( QVariant tick );
    void playFinished();
    void ready();

private:
    static Phonon::VideoPlayer* m_player;

    QWidget*        m_wid;
    QHBoxLayout*    m_layout;

    int m_videoWidth;
    int m_videoHeight;

    bool m_isPreview;
    bool m_isReady;
};

#endif // VIDEO_H
