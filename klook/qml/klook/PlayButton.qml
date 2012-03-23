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

Rectangle {

    //identifier of the item
    id: playButton

    // the color highlight when the mouse hovers on the rectangle
    property color onHoverColor: Qt.darker( borderColor, 1.3 )
    property color borderColor: "#7A8182"

    // buttonColor is set to the button's main color
    property color buttonColor: "#61BDCACD"

    //set appearance properties
    radius: 6
    opacity: 1
    smooth: true
    border.width: 0
    border.color: borderColor
    width: 44; height: 44
    color: "#00000000"
    state: 'Pause'
    anchors.verticalCenter: parent.verticalCenter

    Image{
        id: buttonImage
        source: "images/play.png"
        anchors.fill: parent
        anchors.margins: 6
        smooth: true
        fillMode: Image.PreserveAspectFit
    }

    signal buttonClick()

    //define the clickable area to be the whole rectangle
    MouseArea{
        id: buttonMouseArea
        smooth: true
        anchors.fill: parent    //stretch the area to the parent's dimension

        onClicked: buttonClick()
    }
    states: [
        State {
            name: "Play"
            PropertyChanges { target: buttonImage; source: "images/play.png"; anchors.margins: 6 }
        },
        State {
            name: "Pause"
            PropertyChanges { target: buttonImage; source: "images/pause.png"; anchors.margins: 10 }
        }
    ]

    //scale the button when pressed
    scale: buttonMouseArea.pressed ? 1.05 : 1.00

    //Animate the scale property change
    Behavior on scale { NumberAnimation{ duration: 50 } }
}
