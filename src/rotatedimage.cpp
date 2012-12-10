/* KLook
 * Copyright (c) 2011-2012 ROSA  <support@rosalab.ru>
 * Authors: Evgeniy Auzhin, Sergey Borovkov.
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

#include "rotatedimage.h"

#include <exiv2/exif.hpp>
#include <exiv2/image.hpp>

#include <QtGui/QImage>

RotatedImage::RotatedImage(const QString& path)
    : m_path(path)
{
    rotateImage();
}

void RotatedImage::rotateImage()
{
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(qPrintable(m_path));
    image->readMetadata();

    Exiv2::ExifData &exifData = image->exifData();
    if (exifData.empty())
        return;

    int orientation = 0;
    Exiv2::ExifKey key("Exif.Image.Orientation");
    Exiv2::ExifData::iterator it = exifData.findKey(key);

    if (it == exifData.end())
        return;

    orientation = it->toLong();

    QMatrix rot90 = createRotMatrix(90);
    QMatrix hflip = createScaleMatrix(-1, 1);
    QMatrix vflip = createScaleMatrix(1, -1);

    switch(orientation)
    {
    case NOT_AVAILABLE :
    case NORMAL :
        m_matrix = QMatrix();
        break;
    case HFLIP :
        m_matrix = hflip;
        break;
    case ROT_180:
        m_matrix = createRotMatrix(180);
        break;
    case VFLIP:
        m_matrix = vflip;
        break;
    case TRANSPOSE:
        m_matrix = hflip * rot90;
        break;
    case ROT_90:
        m_matrix = rot90;
        break;
    case TRANSVERSE:
        m_matrix = vflip * rot90;
        break;
    case ROT_270:
        m_matrix = createRotMatrix(270);
        break;
    }
}

QMatrix RotatedImage::createRotMatrix(int angle)
{
    QMatrix matrix;
    matrix.rotate(angle);
    return matrix;
}

QMatrix RotatedImage::createScaleMatrix(int dx, int dy)
{
    QMatrix matrix;
    matrix.scale(dx, dy);
    return matrix;
}

QMatrix RotatedImage::rotationMatrix() const
{
    return m_matrix;
}
