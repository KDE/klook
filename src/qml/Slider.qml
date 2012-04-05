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

Item {
    id: slider; width: 50; height: 10
    smooth: true

    signal posChanged( real pos )

    // value is read/write.
    property real value: 0
    onValueChanged: updatePos();

    property real maximum: 1000
    property real minimum: 0
    property int xMax: width - handle.width - 4
    onXMaxChanged: updatePos();
    onMinimumChanged: updatePos();

    function updatePos()
    {
        if ( maximum > minimum )
        {
            var pos = 2 + ( value - minimum ) * slider.xMax / ( maximum - minimum );
            pos = Math.min( pos, width - handle.width - 2 );
            pos = Math.max( pos, 2 );
            handle.x = pos;
        }
        else
        {
            handle.x = 2;
        }
    }

    Rectangle {
        anchors.fill: parent
        smooth: true

        gradient: Gradient {
            GradientStop { position: 0.0; color: "#183d60" }
            GradientStop { position: 0.5; color: "#234463" }
        }
    }

    MouseArea {
        id: mouseClick
        anchors.fill: parent

        onClicked:
        {
            handle.x = mouseX - handle.width / 2
            value = ( maximum - minimum ) * ( handle.x - 2 ) / slider.xMax + minimum
            posChanged( value )
        }
    }

    Rectangle {
        id: handle; smooth: true
        anchors.verticalCenter: parent.verticalCenter
        width: 14; height:6;

        Image {
            id: handleImage
            anchors.fill: parent
            source: "images/slider.png"
        }

        MouseArea {
            id: mouse
            anchors.fill: parent; drag.target: parent
            drag.axis: Drag.XAxis; drag.minimumX: 2; drag.maximumX: slider.xMax + 2

            onPositionChanged:
            {
                value = ( maximum - minimum ) * ( handle.x - 2 ) / slider.xMax + minimum;
                posChanged( value );
            }
        }
    }
}
