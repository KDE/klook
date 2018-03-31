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

import QtQuick 2.0

Rectangle {
    id: button

    property int buttonHeight: 20
    property int buttonWidth: 100

    // attaches to the Text element's text content
    property string label
    property string st: "normal"
    property string name

    property string  imageUrl: "images/buttons/" + st + "/"+ name + "_" + st + ".png"

    clip: true

    //set appearance properties
    smooth: true
    width: buttonWidth;
    height: buttonHeight
    color: "#00000000"

    Text{
        id: buttonLabel
        anchors.fill: parent
        text: label
        smooth: true     //bind the text to the parent's text
        color: "#000000"
        font.pointSize: 8
        elide: Text.ElideRight
        horizontalAlignment:  Text.AlignHCenter
        verticalAlignment:  Text.AlignVCenter
        z:1
    }

    Image{
        id: buttonImage
        source: imageUrl
        anchors.centerIn: parent
        smooth: true
        fillMode: Image.PreserveAspectFit
    }

    signal buttonClick()

    MouseArea{
        id: buttonMouseArea
        smooth: true
        anchors.fill: parent
        onClicked: buttonClick()
        hoverEnabled: true

        onEntered:
        {
            if ((button.state != 'disabled') && (button.state != 'checked'))
                button.state = "hover"
        }

        onExited:
        {
            if ((button.state != 'disabled') && (button.state != 'checked'))
                button.state = "normal"
        }
    }

    states: [
        State {
            name: "pressed"
            when: mouseArea.pressed === true
            PropertyChanges { target: button; st: "press" }
        },
        State {
            name: "disabled"
            PropertyChanges { target: button; st: "disable" }
            PropertyChanges { target: buttonMouseArea; enabled: false }
        },
        State {
            name: "normal"
            PropertyChanges { target: button; st: "normal" }
        },
        State {
            name: "hover"
            PropertyChanges { target: button; st: "hover" }
        },
        State {
            name: "checked"
            PropertyChanges { target: button; st: "hover" }
        }
    ]

    //scale the button when pressed
    scale: buttonMouseArea.pressed ? 1.05 : 1.00
}
