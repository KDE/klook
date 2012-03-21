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

Item {
    id: gridDelegate;
    width: photosGridView.cellWidth
    height: photosGridView.cellHeight

    Component {
        id: imgVideoDelegate
        Item {
            id: imageItem
            anchors.centerIn: parent
            Preview {

                id: emptyBox
            }
        }
    }

    Component {
        id: txtDelegate

        Item {
            id: txtItem

            DefaultImage {
                id: emptyBox
            }

            signal ready()

            PlainText {
                id: txt
                source: filePath
                anchors.fill: parent
                preview: true
            }
            onReady: { emptyBox.visible = false }
        }
    }

    // function for getting delegate of loader element
    function bestDelegate( t ) {

        //if ( t == 1  || t == 2)
            return imgVideoDelegate;
        //else
            //return txtDelegate;
    }

    Loader {
        id: originalImage
        anchors.centerIn: parent
        width: gridDelegate.width - 20
        height: gridDelegate.height - 20
        sourceComponent: bestDelegate( mimeType )
        //onLoaded: {
            //                console.log( "onLoaded(): w = " + item.width );
       // }
    }
}
