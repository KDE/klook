
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
    id: listItem
    width: photosListView.width; height: photosListView.height

    Component {
        id: imgDelegate

        Item {
            id: imageItem

            Image {
                id: img
                opacity: 0
                anchors.centerIn: parent
                source: filePath
                fillMode: Image.PreserveAspectFit
                asynchronous: true
                smooth: true;
                visible: albumWrapper.state === "fullscreen"
                width: Math.min( sourceSize.width, parent.width )
                height: Math.min( sourceSize.height, parent.height )

                signal ready()

                onStatusChanged: if ( img.status === Image.Ready ){ ready(); opacity = 1; }

                Behavior on opacity { NumberAnimation { duration: 500 } }
            }

            Connections{
                target: photosListView;
                onCurrentIndexChanged: {
                    if ( listItem.ListView.isCurrentItem )
                    {
                        mainWindow.currentFileType = 1;
                        img.opacity = 1
                        mainWindow.updatePanel()
                    } else
                    {
                        img.opacity = 0
                    }
                }
            }
        }
    }

    Component {
        id: videoDelegate

        Item
        {
            id: videoItem

            signal ready()

            Video {
                id: video
                opacity: 0
                anchors.fill: parent
                visible: albumWrapper.state === 'fullscreen' && video.ready

                onTicked:
                {
                    if ( playing )
                    {
                        panel.videoSlider.value = tick * 1000 / video.totalTime; // tick and totalTime in msec
                    }
                }

                onPlayFinished:
                {
                    panel.playButtonState = 'Play'
                    panel.videoSlider.value = 0
                }
                Behavior on opacity { NumberAnimation { duration: 500} }
            }

            Connections{
                target: panel.playItemBtn;
                onButtonClick:
                {
                    if ( listItem.ListView.isCurrentItem )
                    {
                        video.play_or_pause();
                        if ( video.playing )
                            panel.playButtonState = 'Play'
                        else
                            panel.playButtonState = 'Pause'
                    }
                }
            }

            Connections{ target: panel.videoSlider; onPosChanged: video.setPosition( panel.videoSlider.value * video.totalTime / 1000 ) }
            Connections
            {
                target: albumWrapper;
                onStateChanged:
                {
                    if ( albumWrapper.state === "inGrid" )
                        video.pause()
                    else
                        video.play()
                }
            }

            Connections{
                target: photosListView;
                onCurrentIndexChanged: {
                    if ( listItem.ListView.isCurrentItem )
                    {
                        video.opacity = 1
                        video.setPlayerParent()
                        video.source = filePath
                        mainWindow.currentFileType = 2;
                        mainWindow.updatePanel()
                        if ( albumWrapper.state === "fullscreen" )
                            video.play()

                        if ( video.playing )
                            panel.playButtonState = 'Play'
                        else
                            panel.playButtonState = 'Pause'
                    }
                    else
                        video.opacity = 0
                }
            }
        }
    }

    Component {
        id: audioDelegate

        Item
        {
            id: audioItem

            signal ready()

            function getHeight( parentHeight, iconHeight)
            {
                var h = parentHeight * 3 / 5
                if ( h > iconHeight )
                    h = iconHeight
                return h
            }

            Image {
                id: audioIcon
                anchors.left: parent.left
//                anchors.top: parent.top
//                anchors.bottom: parent.bottom
                anchors.verticalCenter: parent.verticalCenter
                anchors.topMargin: 20
                anchors.bottomMargin: 20
                clip: true
                source: "image://mime/" + audio.source
                opacity: 0
                fillMode: Image.PreserveAspectFit
                asynchronous: true
                smooth: true;
                visible: albumWrapper.state === "fullscreen"
                height: getHeight( parent.height, 500 ) //parent.height * 3 / 5

                Behavior on opacity { NumberAnimation { duration: 500 } }
            }

            Text {
                id : title
                font.pointSize: 15
                wrapMode: Text.WordWrap
                color: ( mainWindow.state != "fullscreen" ) ? "black" : "white"
                elide: Text.ElideRight
                anchors.top: audioIcon.top
                anchors.left: audioIcon.right
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.topMargin: 10
                anchors.bottomMargin: 10
            }

            Text {
                id : artist
                wrapMode: Text.WordWrap
                color: ( mainWindow.state != "fullscreen" ) ? "black" : "white"
                elide: Text.ElideRight
                font.pointSize: 12
                anchors.top: title.bottom
                anchors.left: audioIcon.right
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.topMargin: 10
                anchors.bottomMargin: 10
            }

            Text {
                id : totalTime
                wrapMode: Text.WordWrap
                color: ( mainWindow.state != "fullscreen" ) ? "black" : "white"
                elide: Text.ElideRight
                font.pointSize: 12
                anchors.top: artist.bottom
                anchors.left: audioIcon.right
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.topMargin: 10
                anchors.bottomMargin: 10
            }

//            Binding { target: artist; property: "visible"; value: audio.artist != "" }
            Binding { target: totalTime; property: "anchors.top"; value: artist.visible ? artist.bottom : title.bottom }

            Audio {
                id: audio

                onTicked:
                {
                    if ( playing )
                    {
                        panel.videoSlider.value = tick * 1000 / audio.totalTime; // tick and totalTime in msec
                    }
                }

                onPlayFinished:
                {
                    panel.playButtonState = 'Play'
                    panel.videoSlider.value = 0
                }

                onReady:
                {
                    title.text = "<b>" + audio.title + "</b>"
                    artist.text = artistStr + " <b>" + audio.artist + "</b>"
                    artist.visible = ( audio.artist != "" )

                    var h = audio.totalTime / ( 1000 * 3600 )
                    var strFmt = ( h >= 1 ) ? "hh:mm:ss" : "m:ss"

                    totalTime.text = totalTimeStr + " <b>" + Qt.formatTime( audio.duration, strFmt ) + "</b>"
                }
            }

            Connections{
                target: panel.playItemBtn;
                onButtonClick:
                {
                    if ( listItem.ListView.isCurrentItem )
                    {
                        audio.play_or_pause();
                        if ( audio.playing )
                            panel.playButtonState = 'Play'
                        else
                            panel.playButtonState = 'Pause'
                    }
                }
            }

            Connections{ target: panel.videoSlider; onPosChanged: audio.setPosition( panel.videoSlider.value * audio.totalTime / 1000 ) }

            Connections
            {
                target: albumWrapper;
                onStateChanged:
                {
                    if ( albumWrapper.state === "inGrid" )
                        audio.pause()
                    else
                        audio.play()
                }
            }

            Connections{
                target: photosListView;
                onCurrentIndexChanged: {
                    if ( listItem.ListView.isCurrentItem )
                    {
                        audioIcon.opacity = 1
                        audio.source = filePath
                        mainWindow.currentFileType = 4;
                        mainWindow.updatePanel()
                        if ( albumWrapper.state === "fullscreen" )
                            audio.play()

                        if ( audio.playing )
                            panel.playButtonState = 'Play'
                        else
                            panel.playButtonState = 'Pause'
                    }
                    else
                        audioIcon.opacity = 0
                }
            }
        }
    }

    Component {
        id: txtDelegate

        Item {
            id: txtItem
            PlainText {
                id: txt
                source: filePath
                anchors.fill: parent
                preview: false
            }

            Connections{
                target: photosListView;
                onCurrentIndexChanged: {
                    if ( listItem.ListView.isCurrentItem )
                    {
                        mainWindow.currentFileType = 3;
                        mainWindow.updatePanel()
                    }
                }
            }
        }
    }

    Component {
        id: folderDelegate

        Rectangle {
            color: "#dadada"

            Image {
                id: folderIcon
                source: "image://mime/" + filePath
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height / 3 * 2
                anchors.leftMargin: 20
                width: parent.width / 2
                fillMode: Image.PreserveAspectFit

            }

            function getName(path)
            {
                if(path.substr(-1) === '/' && path.length !== 1) { // remove trailing slash if it's not root
                    path = path.substr(0, path.length - 1);
                }
                return path.substring(path.lastIndexOf('/') + 1);
            }

            Text {
                 id: name
                 anchors.left: folderIcon.right
                 anchors.top: folderIcon.top
                 anchors.topMargin: 200
                 text: "<br><b>" + getName(filePath) + "</b></br>" + "<br>Folder</br>"
                 font.pointSize: 12
           }

           Text {
                id: modified
                anchors.left: folderIcon.right
                anchors.top: name.bottom
                text: lastModified
                font.pointSize: 12
            }

           Text {
               id: content
               anchors.top: modified.bottom
               anchors.left: folderIcon.right
               text: contentSize
               font.pointSize: 12
           }
        }
    }



    // function for getting delegate of loader element
    function bestDelegate( t ) {

        if ( t == 1 )
            return imgDelegate;
        else if ( t == 2 )
            return videoDelegate;
        else if ( t == 3 )
            return txtDelegate;
        else if (t == 4) {
            fileModel.scanDirectory(index)
            return folderDelegate;
        }
        else if ( t == 5 )
            return audioDelegate;
        else
            return txtDelegate;
    }

    Loader
    {
        id: componentLoader
        anchors.fill: parent;
        sourceComponent: bestDelegate( mimeType )
    }
}

