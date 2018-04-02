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

import QtQuick 2.7
import QtQuick.Controls 2.3
import Widgets 1.0

Component {
    id: txtDelegate

    Item {
        id: txtItem
        PlainText {
            id: txt
            source: filePath
            //anchors.fill: parent FIXME
            preview: false
        }

        ScrollView {
            clip: true
            anchors.fill: parent
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: txt.preview ? ScrollBar.AsNeeded : ScrollBar.AlwaysOff

            TextArea {
                id: textViewer
                anchors.fill: parent
                text:txt.content
                readOnly: true
            }
        }

        Connections{
            target: photosListView;
            onCurrentIndexChanged: {
                if ( listItem.ListView.isCurrentItem )
                {
                    //Disable scrolling in embedded mode
                    if (mainWindow.state === 'embedded')
                        txt.setPreview(true)
                }
            }
        }
    }
}
