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

            anchors {
                left: parent.left
                leftMargin: leftItemMargin
                top: parent.top
                topMargin: iconHeightMargin
                bottomMargin: iconHeightMargin + panel.height
            }

            source: "image://mime/" + fileUrl
            sourceSize.width: Math.min(parent.width / 3, parent.height)
            sourceSize.height: Math.min(parent.height - anchors.bottomMargin, parent.width)
        }

        InfoItem {
            id: name
            elide: Text.ElideRight
            textFormat: Text.StyledText
            maximumLineCount: 5
            anchors.left: folderIcon.right
            anchors.right: parent.right
            text: "<b>" + fileName +"</b>"
            font.pointSize: 15
            y: (folderIcon.height - folderIcon.paintedHeight) / 2 + iconHeightMargin
        }

        InfoItem {
            id: itemType
            anchors.top: name.bottom
            anchors.left: folderIcon.right
            text: i18n("Folder")
        }

        InfoItem {
            id: modified
            anchors.left: folderIcon.right
            anchors.top: itemType.bottom
            text: (statComplete) ? i18n("Last Modified:") + " " + lastModified : i18n("Last Modified:")
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
            text: (dirSizeComplete)? i18n("Size:") + " " + contentSize : i18n("Size:")
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
            text: (dirSizeComplete)? i18n("Elements:") + " " + countElements : i18n("Elements:")
        }

        AnimatedImage {
            id: contentWaitAnimation
            anchors.top: size.bottom
            anchors.left: content.right
            anchors.leftMargin: 10
            source: "images/loader.gif"
            visible: !dirSizeComplete
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
