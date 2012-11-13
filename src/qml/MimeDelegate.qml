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
        Image {
            id: mimeIcon
            anchors.left: parent.left
            anchors.leftMargin: leftItemMargin
            anchors.top: parent.top
            anchors.topMargin: iconHeightMargin
            anchors.bottom: parent.bottom
            anchors.bottomMargin: iconHeightMargin + panel.height
            source: "image://mime/" + filePath
            clip: true
            fillMode: Image.PreserveAspectFit
            asynchronous: true
            smooth: true;
            visible: albumWrapper.state === ""
            width: getIconWidth( paintedWidth, paintedHeight, getMaxTextWidth() )

            Behavior on opacity { NumberAnimation { duration: 500 } }
        }

        function getMaxTextWidth()
        {
            var w = name.paintedWidth

            if ( w < itemType.paintedWidth )
                w = itemType.paintedWidth
            if ( w < modified.paintedWidth )
                w = modified.paintedWidth
            if ( w < size.paintedWidth )
                w = size.paintedWidth

            return w
        }

        InfoItem {
            id: name
            anchors.left: mimeIcon.right
            text: "<b>" + fileName +"</b>"
            font.pointSize: 15
            y: ( mimeIcon.height - mimeIcon.paintedHeight ) / 2 + iconHeightMargin
        }

        InfoItem {
            id: itemType
            anchors.top: name.bottom
            anchors.left: mimeIcon.right
            text: mime
        }

        InfoItem {
            id: modified
            anchors.top: itemType.bottom
            anchors.left: mimeIcon.right
            text: (statComplete)? i18n("Last Modified:") + " " + lastModified : i18n("Last Modified:")
            elide: Text.ElideNone
            wrapMode: Text.NoWrap
        }

        AnimatedImage {
            id: modifiedWaitAnimation
            anchors.top: itemType.bottom
            anchors.left: modified.right
            anchors.leftMargin: 10
            source: "images/loader.gif"
            visible: !statComplete
        }

        InfoItem {
            id: size
            anchors.top: modified.bottom
            anchors.left: mimeIcon.right
            text:  (statComplete)? i18n("Size:") + " " + contentSize : i18n("Size:")
        }

        AnimatedImage {
            id: sizeWaitAnimation
            anchors.top: modified.bottom
            anchors.left: size.right
            anchors.leftMargin: 10
            source: "images/loader.gif"
            visible: !statComplete
        }

        Connections{
            target: photosListView;
            onCurrentIndexChanged: {
                if ( listItem.ListView.isCurrentItem )
                {
                    mimeIcon.opacity = 1
                } else
                {
                    mimeIcon.opacity = 0
                }
            }
        }

        states: [
            State {
                name: "fullscreen"; when: mainWindow.state === 'fullscreen'
                PropertyChanges { target: modifiedWaitAnimation; source: "images/white-loader.gif" }
                PropertyChanges { target: sizeWaitAnimation; source: "images/white-loader.gif" }
            }
        ]
    }
}
