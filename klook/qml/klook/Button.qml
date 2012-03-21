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
    id: button

    // these properties act as constants, useable outside this QML file
    property int buttonHeight: 20
    property int buttonWidth: 100
    
    // attaches to the Text element's text content
    property string label
    property color textColor: buttonLabel.color

    property string st: "normal"

    property string name

    property string  imageUrl: "images/buttons/" + st + "/"+ name + "_" + st + ".png"; clip: true

    // the color highlight when the mouse hovers on the rectangle
    property color onHoverColor: Qt.darker( borderColor, 1.3 )
    property color borderColor: "#7A8182"

    // buttonColor is set to the button's main color
    property color buttonColor: "#61BDCACD"

    property real labelSize: 12

    //set appearance properties
    smooth: true
    width: buttonWidth;
    height: buttonHeight
    color: "#00000000"
    /*
    gradient: Gradient {
        GradientStop { position: 0.0; color: Qt.lighter( buttonColor, 1.25 ) }
        GradientStop { position: 0.67; color: Qt.darker( buttonColor, 1.3 ) }
    }
  */
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
        //   visible: label != ""
    }

    Image{
        id: buttonImage
        source: imageUrl
        anchors.centerIn: parent
        smooth: true
        fillMode: Image.PreserveAspectFit
    }

    //buttonClick() is callable and a signal handler, onButtonClick is automatically created
    signal buttonClick()

    //define the clickable area to be the whole rectangle
    MouseArea{
        id: buttonMouseArea
        smooth: true
        anchors.fill: parent    //stretch the area to the parent's dimension
        onClicked: buttonClick()
        hoverEnabled: true
        onEntered:
        {
            //console.log(button.state)
            if ((button.state != 'disabled' ) && (button.state != 'checked' ))
            {
                button.state = "hover"
                //   console.log('hover')
            }
        }
        onExited:
        {
            if ((button.state != 'disabled' ) && (button.state != 'checked' ))
            {
                button.state = "normal"
                // console.log('normal')
            }
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

    //change the color of the button when pressed
    //  color: buttonMouseArea.pressed ? Qt.darker(buttonColor, 1.5) : buttonColor

    //animate the color whenever the color property changes
    //  Behavior on color { ColorAnimation{ duration: 55} }

    //scale the button when pressed
    scale: buttonMouseArea.pressed ? 1.05 : 1.00

    //Animate the scale property change
    //    Behavior on scale { NumberAnimation{ duration: 50} }
}
