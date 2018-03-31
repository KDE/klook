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

#include <QObject>

#include <QGraphicsProxyWidget>
#include <KDE/Phonon/VideoPlayer>
#include <KUrl>


class Video : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(bool preview READ isPreview WRITE setPreview)

public:
    Video(QObject* parent = 0);
    ~Video();

public slots:
    QString source() const;
    void setSource(const KUrl& source);

    bool isPreview() const;
    void setPreview(bool preview);

signals:
    void sourceChanged();


private:
    bool m_isPreview;
    KUrl m_source;
};

#endif // VIDEO_H
