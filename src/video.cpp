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


#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QWidget>

#include <KDE/Phonon/AudioOutput>
#include <KDE/Phonon/MediaObject>
#include <KDE/Phonon/VideoWidget>

Video::Video(QObject* parent)
    : QObject(parent)
    , m_isPreview(false)
{

}

Video::~Video()
{

}



QString Video::source() const
{
    return m_source.url();
}

void Video::setSource(const KUrl &source)
{
    m_source = source;
    emit sourceChanged();
}



bool Video::isPreview() const
{
    return m_isPreview;
}

void Video::setPreview(bool preview)
{
    m_isPreview = preview;
}
