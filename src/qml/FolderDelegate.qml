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
            id: folderIcon

            function getMaxTextWidth()
            {
                var w = name.paintedWidth
                if ( w < itemType.paintedWidth )
                    w = itemType.paintedWidth
                if ( w < modified.paintedWidth )
                    w = modified.paintedWidth
                if ( w < size.paintedWidth )
                    w = size.paintedWidth
                if ( w < content.paintedWidth )
                    w = content.paintedWidth

                return w
            }

            anchors {
                left: parent.left
                leftMargin: leftItemMargin
                top: parent.top
                topMargin: iconHeightMargin
                bottom: parent.bottom
                bottomMargin: iconHeightMargin + panel.height
            }

            source: "image://mime/" + fileUrl
            clip: true
            fillMode: Image.PreserveAspectFit
            asynchronous: true
            smooth: true
            visible: albumWrapper.state === ""
            width: getIconWidth( paintedWidth, paintedHeight, getMaxTextWidth() )

            Behavior on opacity { NumberAnimation { duration: 500 } }
        }

        InfoItem {
            id: name
            text: "<b>" + fileName +"</b>"
            anchors.left: folderIcon.right
            font.pointSize: 15
            y: ( folderIcon.height - folderIcon.paintedHeight ) / 2 + iconHeightMargin
        }

        InfoItem {
            id: itemType
            anchors.top: name.bottom
            anchors.left: folderIcon.right
            text: folderStr
        }

        InfoItem {
            id: modified
            anchors.left: folderIcon.right
            anchors.top: itemType.bottom
            text: (statComplete) ? lastModifiedStr + " " + lastModified : lastModifiedStr
            wrapMode: Text.NoWrap
        }

        AnimatedImage {
            id: statWaitAnimation
            anchors.top: itemType.bottom
            anchors.left: modified.right
            anchors.leftMargin: 10
            source: "images/loader.gif"
            visible: !statComplete
        }

        InfoItem {
            id: size
            anchors.top: modified.bottom
            anchors.left: folderIcon.right
            text: (dirSizeComplete)? sizeStr + " " + contentSize : sizeStr
        }

        AnimatedImage {
            id: sizeWaitAnimation
            anchors.top: modified.bottom
            anchors.left: size.right
            anchors.leftMargin: 10
            source: "images/loader.gif"
            visible: !dirSizeComplete
        }

        InfoItem {
            id: content
            anchors.top: size.bottom
            anchors.left: folderIcon.right
            text: (dirSizeComplete)? elementsStr + " " + countElements : elementsStr
        }

        AnimatedImage {
            id: contentWaitAnimation
            anchors.top: size.bottom
            anchors.left: content.right
            anchors.leftMargin: 10
            source: "images/loader.gif"
            visible: !dirSizeComplete
        }

        Connections{
            target: photosListView;
            onCurrentIndexChanged: {
                if ( listItem.ListView.isCurrentItem ) {
                    folderIcon.opacity = 1
                } else {
                    folderIcon.opacity = 0
                }
            }
        }
        states: [
            State {
                name: "fullscreen"; when: mainWindow.state === 'fullscreen'
                PropertyChanges { target: sizeWaitAnimation; source: "images/white-loader.gif" }
                PropertyChanges { target: contentWaitAnimation; source: "images/white-loader.gif" }
                PropertyChanges { target: statWaitAnimation; source: "images/white-loader.gif" }
            }
        ]
    }
}
