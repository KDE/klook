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

Item {
    id: scrollBar;
    // The flickable to which the scrollbar is attached to, must be set
    property variant flickable

    // True for vertical ScrollBar, false for horizontal
    property bool vertical: true

    // If set to false, scrollbar is visible even when not scrolling
    property bool hideScrollBarsWhenStopped: true

    // Thickness of the scrollbar, in pixels
    property int scrollbarWidth: 7

    // value is read/write.
    property real value: 0

    width: vertical ? scrollbarWidth : parent.width
    height: vertical ? parent.height : scrollbarWidth
    x: vertical ? parent.width - width : 0
    y: vertical ? 0 : parent.height - height

    Rectangle {
        id: background
        anchors.fill: parent
        radius: vertical ? (width/2 - 1) : (height/2 - 1)
        color: "black"
        opacity: 0.3
    }

    MouseArea{
        id: scrollMouseArea
        anchors.fill: parent
        hoverEnabled: true

        onEntered: {}
        onExited: {}
        onClicked:
        {
            if (vertical)
            {
                var cY = (flickable.contentHeight * mouseY / height)
                var pagePsize = flickable.contentHeight * flickable.visibleArea.heightRatio
                if (cY > (flickable.contentHeight - pagePsize))
                {
                    cY = flickable.contentHeight - pagePsize
                }
                var rowPsize = Math.round(flickable.contentHeight*flickable.visibleArea.heightRatio) / 4
                var numRow = Math.round(cY / rowPsize)
                var curNumRow = Math.round(flickable.contentY / rowPsize)
                if (numRow === curNumRow)
                {
                    if (mouseY > scrollBarHandle.Y)
                        numRow++
                    else
                        numRow--
                }
                flickable.contentY = numRow * rowPsize
            }
            else
            {
                scrollBarHandle.x = mouseX - scrollBarHandle.width / 2
            }
        }
    }

    Rectangle {
        id: scrollBarHandle
        color: "white"
        radius: vertical ? (width / 2 - 1) : (height / 2 - 1)

        function sbOpacity()
        {
            if ( !hideScrollBarsWhenStopped ) {
                return 0.5;
            }

            return ( flickable.flicking || flickable.moving ) ? ( vertical ? ( height >= parent.height ? 0 : 0.5 ) : ( width >= parent.width ? 0 : 0.5 ) ) : 0;
        }

        function returnX()
        {
            //Stop calculating x while handle is dragged
            if(scrollHandleMouseArea.drag.active)
                return scrollHandleMouseArea.drag.target.x
            var x
            x =  vertical ? parent.width - width : flickable.visibleArea.xPosition * parent.width
            if (x < 0) {
                x = 0
            }
            else if (!vertical && ((x + scrollBarHandle.width) > flickable.widht)) {
                x = flickable.widht - scrollBarHandle.width
            }

            return x
        }

        function returnY()
        {
            //Stop calculating y while handle is dragged
            if ( scrollHandleMouseArea.drag.active )
                return scrollHandleMouseArea.drag.target.y
            var y
            y = vertical ? flickable.visibleArea.yPosition * parent.height : parent.height - height
            if (y < 0)
                y = 0
            else if (vertical && ((y + scrollBarHandle.height) > flickable.height)) {
                y = flickable.height - scrollBarHandle.height
            }

            return y
        }

        // Scrollbar appears automatically when content is bigger than the Flickable
        opacity: sbOpacity()

        // Calculate width/height and position based on the content size and position of
        // the Flickable
        width: vertical ? scrollbarWidth : flickable.visibleArea.widthRatio * parent.width
        height: vertical ? flickable.visibleArea.heightRatio * parent.height : scrollbarWidth
        x: returnX()
        y: returnY()

        // Animate scrollbar appearing/disappearing
        Behavior on opacity { NumberAnimation { duration: 500 } }

        MouseArea{
            id: scrollHandleMouseArea

            function snapToRow()
            {
                var y = flickable.contentY
                var numRow = y / ((flickable.contentHeight*flickable.visibleArea.heightRatio) / 4)
                numRow = Math.round(numRow)
                return numRow * Math.round((flickable.contentHeight * flickable.visibleArea.heightRatio) / 4)
            }

            anchors.fill: parent
            hoverEnabled: true
            drag.filterChildren: true
            drag.target: parent;
            drag.axis: vertical ? Drag.YAxis : Drag.XAxis
            drag.minimumY: 0
            drag.maximumY: flickable.height - scrollBarHandle.height

            onMousePositionChanged:
            {
                if (drag.active) {
                    flickable.contentY = flickable.contentHeight * scrollBarHandle.y / scrollBar.height
                }
            }

            onReleased:{
                flickable.contentY = snapToRow()
            }
        }
    }
}
