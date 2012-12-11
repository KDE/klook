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

import QtQuick 1.1
import Widgets 1.0

Component {
    Rectangle {
        id: imageItem
        color: "#333333"

        Image {
            id: img
            anchors.centerIn: parent
            signal ready()

            sourceSize {
                width: parent.width
                height: parent.height
            }

            source: "image://exif/" + filePath
            fillMode: Image.PreserveAspectFit
            asynchronous: true
            smooth: true
            opacity: 0
            visible: albumWrapper.state === ""

            onStatusChanged: if (img.status === Image.Ready) { ready(); opacity = 1; }

            Behavior on opacity { NumberAnimation { duration: 500 } }
        }

        Connections{
            target: photosListView;
            onCurrentIndexChanged: {
                img.opacity = listItem.ListView.isCurrentItem ? 1 : 0
            }
        }
    }
}
