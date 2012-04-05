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

import QtQuick 1.0

// This compoment has few ugly workarounds because of Qt QTBUG-14900
// because of this bug it's very hard to update image using image provider
// so we add some random symbols to file name every time we are interested in image
// update

Image {
    source: "image://preview/" + filePath + "%" + Math.random( 10 )
    width: Math.min( sourceSize.width, parent.width )
    height: Math.min( sourceSize.height, parent.height )
    fillMode: Image.PreserveAspectFit
    anchors.centerIn: parent
    smooth: true
    asynchronous: true
    visible: albumWrapper.state === "inGrid"

    Connections
    {
        target: fileModel
        onDataChanged:
        {
            source = "image://preview/" + filePath + "%" + Math.random( 10 )
        }
    }
}
