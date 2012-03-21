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

Rectangle{
    id: controlPanel
    width: 200
    height: 66
    opacity: 1
    color: "#042443"
    radius: 5
    border.width: 1
    border.color: "#3378b9"
    clip: true

    anchors.leftMargin: 20
    anchors.rightMargin: 20

    //state: "videoPanel"

    //anchors.bottom: parent.bottom;
    //anchors.bottomMargin: 20
    anchors.horizontalCenter: parent.horizontalCenter

    property alias videoSlider: slider
    property alias playItemBtn: playItemButton
    property alias playButtonState: playItemButton.state

    property int margin: 10

    gradient: Gradient {
        GradientStop { position: 0.0; color: "#1c60a0" }
        GradientStop { position: 0.5; color: "#042443"}
    }


    Rectangle {
        id: controlPanelSmall
        //x:0
        //y:0
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.rightMargin: 1

         clip: true

        height: 49
        //swidth: 200
        //color: "#000000"
        color: "transparent"
        smooth: true
        //opacity: 1
        radius: 5
        border.width: 1
        border.color: "#3378b9"
        //border.color: "#ffffff"

        ControlPanelButton
        {
            id: prevItemButton
            name: 'go-previous'
            onButtonClick:
            {
                photosListView.decrementCurrentIndex()
                //console.log( "Go previuous was clicked..." )
                controlPanel.state = mainWindow.updatePanelState()
                //mainWindow.updatePanel()
            }
            Behavior on x { NumberAnimation { duration: 100 } }
        }

        PlayButton { id: playItemButton }

        ControlPanelButton
        {
            id: nextItemButton
            name: 'go-next'
            onButtonClick:
            {
                if ( photosListView.currentIndex === -1 )
                    photosListView.currentIndex = 0
                photosListView.incrementCurrentIndex()

                //console.log( "Go-next was clicked..." )
                controlPanel.state = mainWindow.updatePanelState()
                //mainWindow.updatePanel()
            }
            //Behavior on x { NumberAnimation { duration: 100 } }
        }

        ControlPanelButton
        {
            id: galleryItemButton
            name: 'gallery'
            onButtonClick:
            {
                setGallery()
                //console.log( "Gallery was clicked..." )
                controlPanel.state = mainWindow.updatePanelState()
                mainWindow.updatePanel()
            }
            //Behavior on x { NumberAnimation { duration: 100 } }
        }

        Image
        {
            id: separator
            source: "images/separator.png"
            height: parent.height - 10
            anchors.verticalCenter: parent.verticalCenter
            //anchors.right: resumeScreenButton.left
            //Behavior on x { NumberAnimation { duration: 100 } }
        }

        ControlPanelButton
        {
            id: resumeScreenButton
            name: 'resume'
            onButtonClick:
            {
                setFullScreen()
                //console.log( "resumeScreenButton was clicked..." )
            }
            //Behavior on x { NumberAnimation { duration: 100 } }
        }

        ControlPanelButton
        {
            id: closeButton
            name: 'close'
            onButtonClick: Qt.quit()
            //Behavior on x { NumberAnimation { duration: 100 } }
        }
    }

    Slider {
        id: slider
        x: 20
        y: 54

        //anchors.bottom: parent.bottom
        width: controlPanel.width - 4 * margin //- playItemButton.width
        height: 6
        Behavior on y
        {
            NumberAnimation{duration: 400}
        }
    }

    states: [
        State {
            name: "videoPanel"; when: ( ( mainWindow.state === "windowed" ) || ( mainWindow.state === "embedded" ) )
            PropertyChanges { target: controlPanel; height: 66 }
            PropertyChanges { target: controlPanel; border.width: 1 }
            PropertyChanges { target: prevItemButton; visible: false }
            PropertyChanges { target: playItemButton; visible: true }
            PropertyChanges { target: nextItemButton; visible: false }
            PropertyChanges { target: galleryItemButton; visible: false }
            PropertyChanges { target: resumeScreenButton; visible: false }
            PropertyChanges { target: closeButton; visible: false }
            PropertyChanges { target: playItemButton; x: (controlPanel.width/2 - (playItemButton.width/2)) }
            PropertyChanges { target: slider; visible: true }
            PropertyChanges { target: slider; y: 54 }
            PropertyChanges { target: controlPanel; width: 200/*controlPanel.parent.width - 2 * margin*/ }
            PropertyChanges { target: separator; visible: false }
        },
        State {
            name: "fullscreenPanelSingle"
            PropertyChanges { target: controlPanel; height: 50 }
            PropertyChanges { target: controlPanel; border.width: 0 }
            PropertyChanges { target: prevItemButton; visible: false }
            PropertyChanges { target: playItemButton; visible: false }
            PropertyChanges { target: nextItemButton; visible: false }
            PropertyChanges { target: galleryItemButton; visible: false }
            PropertyChanges { target: slider; visible: false }
            PropertyChanges { target: slider; y: 50 }
            PropertyChanges { target: resumeScreenButton; visible: true }
            PropertyChanges { target: closeButton; visible: true }
            PropertyChanges { target: resumeScreenButton; x: margin }
            PropertyChanges { target: closeButton; x: resumeScreenButton.x + resumeScreenButton.width }
            PropertyChanges { target: controlPanel; width: resumeScreenButton.width + closeButton.width + 2 * margin }
            PropertyChanges { target: separator; visible: false }
        },
        State {
            name: "fullscreenPanelMulti"; extend: "fullscreenPanelSingle"
            PropertyChanges { target: prevItemButton; visible: true }            
            PropertyChanges { target: nextItemButton; visible: true }
            PropertyChanges { target: galleryItemButton; visible: true }
            PropertyChanges { target: prevItemButton; x: margin }
            PropertyChanges { target: nextItemButton; x: prevItemButton.x + prevItemButton.width }
            PropertyChanges { target: galleryItemButton; x: nextItemButton.x + nextItemButton.width }
            PropertyChanges { target: separator; x: galleryItemButton.x + galleryItemButton.width + margin }
            PropertyChanges { target: resumeScreenButton; x: separator.x + separator.width + margin }
            PropertyChanges { target: closeButton; x: resumeScreenButton.x + resumeScreenButton.width }
            PropertyChanges { target: controlPanel; width: prevItemButton.width + nextItemButton.width + galleryItemButton.width + separator.width +
                                                           resumeScreenButton.width + closeButton.width + 4 * margin }
            PropertyChanges { target: separator; visible: true }
        },
        State {
            name: "fullscreenVideoPanelSingle"
            PropertyChanges { target: controlPanel; height: 66 }
            PropertyChanges { target: controlPanel; border.width: 1 }
            PropertyChanges { target: prevItemButton; visible: false }
            PropertyChanges { target: playItemButton; visible: true }
            PropertyChanges { target: nextItemButton; visible: false }
            PropertyChanges { target: galleryItemButton; visible: false }
            PropertyChanges { target: resumeScreenButton; visible: true }
            PropertyChanges { target: closeButton; visible: true }
            PropertyChanges { target: playItemButton; x: margin }
            PropertyChanges { target: separator; x: playItemButton.x + playItemButton.width + margin}
            PropertyChanges { target: resumeScreenButton; x: separator.x + separator.width + margin }
            PropertyChanges { target: closeButton; x: resumeScreenButton.x + resumeScreenButton.width }
            PropertyChanges { target: controlPanel; width: playItemButton.width + resumeScreenButton.width + separator.width +
                                                           closeButton.width + 4 * margin }
            PropertyChanges { target: separator; visible: true }
            PropertyChanges { target: slider; visible: true }
            PropertyChanges { target: slider; y: 54 }
        },
        State {
            name: "fullscreenVideoPanelMulti"; extend: "fullscreenVideoPanelSingle"
            PropertyChanges { target: prevItemButton; visible: true }
            PropertyChanges { target: nextItemButton; visible: true }
            PropertyChanges { target: galleryItemButton; visible: true }
            PropertyChanges { target: prevItemButton; x: margin }
            PropertyChanges { target: playItemButton; x: prevItemButton.x + prevItemButton.width }
            PropertyChanges { target: nextItemButton; x: playItemButton.x + playItemButton.width }
            PropertyChanges { target: galleryItemButton; x: nextItemButton.x + nextItemButton.width }
            PropertyChanges { target: separator; x: galleryItemButton.x + galleryItemButton.width + margin }
            PropertyChanges { target: resumeScreenButton; x: separator.x + separator.width + margin }
            PropertyChanges { target: closeButton; x: resumeScreenButton.x + resumeScreenButton.width }
            PropertyChanges { target: controlPanel; width: prevItemButton.width + playItemButton.width + nextItemButton.width +
                                                           galleryItemButton.width + separator.width + resumeScreenButton.width +
                                                           closeButton.width + margin * 4 }
        }
    ]

    // Вставляем задержку на изменение прозрачности
    Behavior on opacity { NumberAnimation { duration: 500 } }
    Behavior on height { NumberAnimation { duration: 300 } }
    //Behavior on width { NumberAnimation { duration: 300 } }

}
