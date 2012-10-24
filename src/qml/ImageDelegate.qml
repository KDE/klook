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
    Item {
        id: imageItem

        Image {
            id: img
            opacity: 0
            anchors.centerIn: parent
            source: filePath
            fillMode: Image.PreserveAspectFit
            asynchronous: true
            smooth: true;
            visible: albumWrapper.state === "fullscreen"
            width: Math.min( sourceSize.width, parent.width )
            height: Math.min( sourceSize.height, parent.height )

            signal ready()

            onStatusChanged: if ( img.status === Image.Ready ) { ready(); opacity = 1; }

            Behavior on opacity { NumberAnimation { duration: 500 } }
        }

        Connections{
            target: photosListView;
            onCurrentIndexChanged: {
                if ( listItem.ListView.isCurrentItem )
                {
                    img.opacity = 1
                } else
                {
                    img.opacity = 0
                }
            }
        }
    }
}
