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

Rectangle {
    id: mainWindow
    width: 600
    height: 400
    color: "transparent"
    border.width: 2
    clip: true
    border.color: "#7b7b7b"
    smooth: true
    focus: true
    state: ( embedded ? 'embedded' : 'windowed' )
    anchors.fill: parent
    anchors.leftMargin: 0
    anchors.rightMargin: 0
    anchors.bottomMargin: 0

    property bool firstFileLoaded: false
    property int currentFileType

    property alias mainState: mainWindow.state
    property alias wrapperState: albumWrapper.state

    signal appendItem( string path, int type )
    signal setGalleryView( bool isGallery )

    function setFullScreen()
    {
        if ( mainWindow.state === 'fullscreen' )
            mainWindow.state = 'windowed'
        mainWidget.setFullScreen()
    }

    function setFullScreenState()
    {
        mainWindow.state = 'fullscreen'
    }

    function setEmbeddedState()
    {
        mainWindow.state = 'embedded'
    }

    function setStartWindow()
    {
        if ( embedded )
            mainWindow.state = 'embedded'
        else
            mainWindow.state = 'windowed'
        albumWrapper.state = ""
        photosListView.focus = true
    }

    function updatePanelState()
    {
        if ( mainWindow.state === "windowed" )
            return 'videoPanel'

        if ( viewMode === "multi" )
            if ((mainWindow.currentFileType === File.Video) || (mainWindow.currentFileType === File.Audio))
                return 'fullscreenVideoPanelMulti'
            else
                return 'fullscreenPanelMulti'
        else
            if ((mainWindow.currentFileType === File.Video) || (mainWindow.currentFileType === File.Audio))
                return 'fullscreenVideoPanelSingle'
            else
                return 'fullscreenPanelSingle'
    }

    function updatePanel()
    {
        controlPanelTimer.stop()
        panel.opacity = 1
        //Hide panel in gallery
        if ( albumWrapper.state === "inGrid" ) {
            panel.visible = false
            return
        }
        //Hide panel in normal mode if content isnt video or audio
        if (( mainWindow.state === "windowed" ) && (mainWindow.currentFileType !== File.Video) && (mainWindow.currentFileType !== File.Audio)) {
            panel.visible = false
            return
        }
        panel.visible = true
        if ( !mouseControl.containsMouse ) {
            controlPanelTimer.start()
        }
    }

    // calculates size of grid item
    function getCellSize( countItems )
    {
        var w = ( countItems > 20 ) ? ( galleryGridView.width - scrollBar.width * 2 ) : ( galleryGridView.width - 2 )
        var h = galleryGridView.height

        if ( countItems <= 1 ) return Qt.size( w, h )
        else if ( countItems <= 2 ) return Qt.size( w / 2, h )
        else if ( countItems <= 4 ) return Qt.size( w / 2, h / 2 )
        else if ( countItems <= 6 ) return Qt.size( w / 3, h / 2 )
        else if ( countItems <= 9 ) return Qt.size( w / 3, h / 3 )
        else if ( countItems <= 12 ) return Qt.size( w / 4, h / 3 )
        else if ( countItems <= 16 ) return Qt.size( w / 4, h / 4 )
        else return Qt.size( w / 5, h / 4 )
    }

    function updateMenuButtons()
    {
        prevButton.state = getPrevButtonState()
        nextButton.state = getNextButtonState()
        panel.prevButtonState = getPrevButtonState()
        panel.nextButtonState = getNextButtonState()
    }

    function getPrevButtonState()
    {
        if ( photosListView.currentIndex === 0 )
            return "disabled"
        else
            return "normal"
    }

    function getNextButtonState()
    {
        if ( photosListView.currentIndex === ( photosListView.count - 1 ) )
            return "disabled"
        else
            return "normal"
    }

    Keys.onEscapePressed: {
        if ( mainWindow.state === 'fullscreen' )
            setFullScreen();
        else
            Qt.quit()
    }

    Keys.onSpacePressed:
    {
        Qt.quit()
    }

    Keys.onReturnPressed:
    {
        if (galleryGridView.currentIndex !== -1)
            albumWrapper.state = ""

        setGalleryView( albumWrapper.state === 'inGrid' )
    }

    Component.onCompleted:
    {
        if ( embedded )
            mainWindow.state = 'embedded'
        else
            mainWindow.state = 'windowed'
    }

    function setGallery()
    {
        if ( photosListView.currentIndex === -1 )
            photosListView.currentIndex = 0

        if ( albumWrapper.state === '' )
        {
            albumWrapper.state = 'inGrid'
            galleryGridView.currentIndex = photosListView.currentIndex
        }
        else
        {
            if ( galleryGridView.currentIndex === -1 )
            {
                galleryGridView.currentIndex = photosListView.currentIndex
            }
            albumWrapper.state = ""
        }

        setGalleryView( albumWrapper.state === 'inGrid' )
    }

    Image {
        id: arrow
        source: "images/arrow/arrow-left.png"
        z:100
        visible: false
    }

    // Item 1: menu bar
    Rectangle {
        id: menu
        height: 27
        color: "#dadada"
        anchors.right: parent.right; anchors.rightMargin: 1
        anchors.left: parent.left; anchors.leftMargin: 1
        anchors.top: parent.top; anchors.topMargin: 1
        transformOrigin: Item.Center
        z: 10

        gradient: Gradient {
            GradientStop { position: 0.0; color: Qt.lighter( "#dadada", 1.1 ) }
            GradientStop { position: 1; color: Qt.darker( "#dadada", 1 ) }
        }

        Button {
            id: prevButton
            buttonWidth: 42
            buttonHeight: 22
            anchors.left: parent.left; anchors.leftMargin: 6; anchors.verticalCenter: parent.verticalCenter
            name: 'prev'
            visible: viewMode === "multi"
            z: 1
            onButtonClick:
            {
                photosListView.decrementCurrentIndex()
                updateMenuButtons()
            }
        }

        Button {
            id: nextButton
            buttonWidth: 42
            buttonHeight: 22
            anchors.left: prevButton.right; anchors.leftMargin: 0; anchors.verticalCenter: parent.verticalCenter
            name: 'next'
            visible: viewMode === "multi"

            onButtonClick:
            {
                if ( photosListView.currentIndex === -1 )
                    photosListView.currentIndex = 0
                photosListView.incrementCurrentIndex()
                updateMenuButtons()
            }
        }

        Button {
            id: galleryButton
            buttonWidth: 42
            buttonHeight: 22
            anchors.left: nextButton.right; anchors.leftMargin: 12; anchors.verticalCenter: parent.verticalCenter
            name: 'gallery'
            visible: viewMode === "multi"
            state: 'normal'

            onButtonClick:
            {
                setGallery()
            }
        }

        Text {
            id: fileNameLabel
            anchors.verticalCenter: nextButton.verticalCenter
            text: fileName
            anchors.left: ( viewMode === "multi" ) ? galleryButton.right : parent.left
            anchors.leftMargin: 6
            anchors.right: openButton.left
            anchors.rightMargin: 6
            color: "#000000"
            font.pointSize: 8
            elide: Text.ElideMiddle
            horizontalAlignment:  Text.AlignHCenter
            verticalAlignment:  Text.AlignVCenter
        }

        Button {
            id: fullscreenButton
            buttonWidth: 42
            buttonHeight: 22
            anchors.right: quitButton.left; anchors.rightMargin: 0; anchors.verticalCenter: parent.verticalCenter
            name: 'fullscreen'

            onButtonClick:{
                setFullScreen()
            }

        }

        Button {
            id: quitButton
            buttonWidth: 42
            buttonHeight: 22
            anchors.right: parent.right; anchors.rightMargin: 6; anchors.verticalCenter: parent.verticalCenter
            name: 'close'

            onButtonClick: Qt.quit()
        }

        Button {
            id: openButton
            buttonWidth: 168
            buttonHeight: 22
            anchors.right: fullscreenButton.left
            anchors.rightMargin: 12
            anchors.verticalCenterOffset: 0
            anchors.verticalCenter: parent.verticalCenter
            name: 'open_in'
            label: openText

            onButtonClick: {
                Qt.openUrlExternally( fileUrl )
                Qt.quit()
            }
        }
    }

    Rectangle{
        id: drawerBorder
        smooth: true
        anchors.right: parent.right
        anchors.rightMargin: 1
        anchors.left: parent.left
        anchors.leftMargin: 1
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 1
        anchors.top: menu.bottom
        anchors.topMargin: -1
        border.width: 0
        border.color: "#7b7b7b"

        Rectangle {
            id: drawer
            clip:true
            anchors.rightMargin: 2
            anchors.leftMargin: 2
            anchors.bottomMargin: 2
            anchors.topMargin: 2
            anchors.fill: parent
            color:  "#dadada"
            border.width: 0
            border.color: "#000000"

            Rectangle {
                id: albumsShade; color: mainWindow.color;  width: parent.width; height: parent.height; opacity: 0.0
            }

            Item {
                id: albumWrapper;
                anchors.fill: parent

                Component {
                    id: highlight
                    Rectangle {
                        width: galleryGridView.cellWidth; height: galleryGridView.cellHeight
                        color: "#c8b0c4de"; radius: 5
                        x: (galleryGridView.currentIndex !== -1) ? galleryGridView.currentItem.x : 0
                        y: (galleryGridView.currentIndex !== -1) ? galleryGridView.currentItem.y : 0
                        z: 0
                        Behavior on x { SpringAnimation { spring: 1; damping: 0.2 } }
                        Behavior on y { SpringAnimation { spring: 1; damping: 0.2 } }
                    }
                }

                GridView {
                    id: galleryGridView
                    width: drawer.width;
                    height: drawer.height
                    anchors.fill: parent ; anchors.margins: 10
                    model: fileModel
                    delegate: Delegate{}
                    cellWidth: getCellSize( galleryGridView.count ).width
                    cellHeight: getCellSize( galleryGridView.count ).height
                    highlight:  highlight
                    highlightFollowsCurrentItem: false
                    snapMode: GridView.SnapToRow
                    clip: true
                    visible: false

                    onCurrentIndexChanged:
                    {
                        mainWidget.updateMenu(currentIndex)
                    }


                    MouseArea {
                        id: mouseAreaGrid
                        anchors.fill: parent
                        anchors.rightMargin: 20
                        anchors.bottomMargin: 30
                        hoverEnabled: true
                        z: 20

                        onMousePositionChanged:
                        {
                            var mouseIndex = galleryGridView.indexAt(mouseX + galleryGridView.contentX, mouseY + galleryGridView.contentY)
                            if (mouseIndex !== -1)
                            {
                                if (openButton.state !== 'normal')
                                    openButton.state = 'normal'
                                galleryGridView.currentIndex = mouseIndex
                            }
                            else
                            {
                                if (openButton.state !== 'disabled')
                                    openButton.state = 'disabled'
                                galleryGridView.currentIndex = -1
                            }
                            mainWidget.updateMenu(mouseIndex)
                        }

                        onClicked: {
                            if ( ( albumWrapper.state == 'inGrid' ) && ( galleryGridView.currentIndex !== -1 ) ) {
                                albumWrapper.state = ""
                            }
                            setGalleryView( albumWrapper.state === 'inGrid' )
                            panel.state = mainWindow.updatePanelState()
                        }

                    }
                    Behavior on contentY {
                        NumberAnimation { duration: 300 }
                    }

                    ScrollBar{
                        id: scrollBar
                        flickable: galleryGridView
                        vertical: true
                        hideScrollBarsWhenStopped: false
                        scrollbarWidth: 10
                        z:30
                        visible: galleryGridView.count > 20
                    }
                }

                ListView {
                    id: photosListView
                    model: fileModel
                    delegate: SingleDelegate{}
                    orientation: Qt.Horizontal
                    anchors.fill: parent
                    anchors.margins: 0
                    spacing: 200
                    clip: true
                    interactive: false
                    focus: true
                    highlightFollowsCurrentItem: true
                    highlightRangeMode: ListView.StrictlyEnforceRange;
                    highlightMoveSpeed: 5000
                    preferredHighlightBegin: 0; preferredHighlightEnd: 0  //this line means that the currently highlighted item will be central in the view

                    onCurrentIndexChanged:
                    {
                        fileModel.load(currentIndex)
                        updateMenuButtons()
                        updatePanel()
                    }

                }

                Keys.onLeftPressed:
                {
                    if ( photosListView.focus === true )
                        photosListView.decrementCurrentIndex()
                }

                Keys.onRightPressed:
                {
                    if ( photosListView.focus === true )
                    {
                        if ( photosListView.currentIndex === -1 )
                            photosListView.currentIndex = 0
                        photosListView.incrementCurrentIndex()
                    }
                }

                Connections {
                    target: fileModel
                    onRowsInserted: {
                        if (fileModel.count() !== 0) {
                            updateMenuButtons()
                            photosListView.positionViewAtIndex(indexToShow, ListView.Contain)
                        }
                    }
                }
                states: [
                    State {
                        name: "inGrid"
                        PropertyChanges { target: albumsShade; opacity: 1 }
                        PropertyChanges { target: galleryGridView; visible: true }
                        PropertyChanges { target: galleryGridView; focus: true }
                        PropertyChanges { target: photosListView; visible: false }
                        PropertyChanges { target: photosListView; focus: false  }                        
                    }
                ]

                transitions: [
                    Transition {
                        from: "inGrid"; to: "*"
                        NumberAnimation { properties: "y,opacity"; easing.type: Easing.OutQuad; duration: 100 }
                    }
                ]

                onStateChanged:
                {                    
                    if (albumWrapper.state === "inGrid") {
                        previewGenerator.start()
                        prevButton.state = 'disabled'
                        nextButton.state = 'disabled'
                    } else {
                        previewGenerator.stop()
                        photosListView.currentIndex = galleryGridView.currentIndex
                        photosListView.positionViewAtIndex( photosListView.currentIndex, ListView.Contain )
                        updateMenuButtons()
                    }
                }
            }

            ControlPanel {
                id: panel
                z: 1
                y: albumWrapper.height - 70

                Connections{
                    target: mainWindow
                    onStateChanged: {
                        updatePanel()
                    }
                }

                Connections{
                    target: albumWrapper
                    onStateChanged: {
                        updatePanel()
                    }
                }
            }

            Timer {
                id : controlPanelTimer
                interval: 2000; running: false;
                repeat: false

                onTriggered: panel.opacity = 0
            }

            MouseArea {
                id: mouseControl
                width:panel.width
                height: panel.height
                anchors.fill: panel
                hoverEnabled: true

                onEntered: {
                    controlPanelTimer.stop()
                    panel.opacity = 1
                }

                onExited: {
                    controlPanelTimer.start()
                }
            }

            Item {
                id: foreground;
                anchors.fill: parent
            }
        }
    }

    states: [
        State {
            name: "windowed"
            PropertyChanges {
                target: mainWindow;
                border.width: 2
                color: "transparent"
            }
            PropertyChanges { target: menu; visible: true }
            PropertyChanges { target: photosListView; highlightMoveSpeed: 5000 }
            PropertyChanges {
                target: panel
                opacity: 1
                state: "videoPanel"
                y: albumWrapper.height - panel.height - 10
            }
            ParentChange { target: drawer; parent: drawerBorder }
            PropertyChanges {
                target: drawer
                anchors.margins: 1
                anchors.rightMargin: 2
                anchors.leftMargin: 2
                anchors.bottomMargin: 2
                anchors.topMargin: 1
            }

            PropertyChanges {
                target: drawerBorder
                color: "#dadada"
                visible: true
            }

            PropertyChanges {
                target: panel;
                state: updatePanelState()
                y: albumWrapper.height - panel.height - 32
            }

        },
        State {
            name: "fullscreen"
            PropertyChanges { target: mainWindow; border.width: 0 }

            // this is just for a case when okular delegate was displayed in windowed mode
            PropertyChanges { target: mouseControl; enabled: true }

            PropertyChanges { target: photosListView; highlightMoveSpeed: 7000 }
            PropertyChanges {
                target: panel;
                state: updatePanelState()
                y: albumWrapper.height - panel.height - 32
            }

            PropertyChanges { target: menu; visible: false }
            ParentChange { target: drawer; parent: mainWindow }
            PropertyChanges {
                target: drawer
                anchors.rightMargin: 0
                anchors.leftMargin: 0
                anchors.bottomMargin: 0
                anchors.topMargin: 0
                color: "#333333"
            }
            PropertyChanges { target: drawerBorder; visible: false }
        },
        State {
            name: "embedded"
            PropertyChanges {target: mainWindow;  border.width: 0; }

            PropertyChanges { target: menu; visible: false  }

            AnchorChanges   { target: drawerBorder; anchors.top: parent.top }
            PropertyChanges { target: drawerBorder; anchors.topMargin: 0 }
            PropertyChanges { target: drawer; anchors.topMargin: 1 }
            PropertyChanges {
                target: drawerBorder
                color: "#537492"
                anchors.rightMargin: ( embeddedLayout === "left" ) ? 16 : 0
                anchors.leftMargin: ( embeddedLayout === "right" ) ? 16 : 0
                anchors.bottomMargin : ( embeddedLayout === "top" ) ? 16 : 0
            }
            PropertyChanges {
                target: arrow
                source: (embeddedLayout === "top") ?
                            "images/arrow/arrow-bottom.png" :
                            ((embeddedLayout === "left") ? "images/arrow/arrow-right.png" : "images/arrow/arrow-left.png")
                visible : true
                x : arrowX; y: arrowY
            }
            PropertyChanges {
                target: drawer
                anchors.margins: 1
                anchors.rightMargin: 1
                anchors.leftMargin: 1
                anchors.bottomMargin: 1
                anchors.topMargin: 1
            }
        }
    ]
}
