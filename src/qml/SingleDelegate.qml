
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

    function getHeight( parentHeight, iconHeight)
    {
        var h = ( parentHeight - panel.height ) * 4 / 5
        if ( h > iconHeight )
            h = iconHeight
        return h
    }

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

                onStatusChanged: if ( img.status === Image.Ready ) { ready(); opacity = 1; }

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

            Image {
                id: audioIcon
                anchors.left: parent.left
                clip: true
                source: "images/audio.png"
                opacity: 0
                fillMode: Image.PreserveAspectFit
                asynchronous: true
                smooth: true;
                visible: albumWrapper.state === "fullscreen"
                height: getHeight( parent.height, 500 ) //parent.height * 3 / 5
                y: ( parent.height - panel.height - height ) / 2

                Behavior on opacity { NumberAnimation { duration: 500 } }
            }

            InfoItem {
                id : title
                font.pointSize: 15
                anchors.top: audioIcon.top
                anchors.left: audioIcon.right
                anchors.right: parent.right
            }

            InfoItem {
                id : artist
                anchors.top: title.bottom
                anchors.left: audioIcon.right
            }

            InfoItem {
                id : totalTime
                anchors.top: artist.bottom
                anchors.left: audioIcon.right
            }

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

            Connections{
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
                        mainWindow.currentFileType = 5;
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
                        //Disable scrolling in embedded mode
                        if (mainWindow.state === 'embedded')
                            txt.setPreview(true)
                    }
                }
            }
        }
    }

    function getName( path )
    {
        if ( path.substr( -1 ) === '/' && path.length !== 1 ) { // remove trailing slash if it's not root
            path = path.substr( 0, path.length - 1 );
        }
        return path.substring( path.lastIndexOf( '/' ) + 1 );
    }

    Component {
        id: folderDelegate

        Item {
            id: folderItem

            Image {
                id: folderIcon
                source: "image://mime/" + filePath
                clip: true
                anchors.left: parent.left
                anchors.leftMargin: 20
                fillMode: Image.PreserveAspectFit
                asynchronous: true
                smooth: true;
                visible: albumWrapper.state === "fullscreen"
                height: getHeight( parent.height, 500 )
                y: ( parent.height - panel.height - height ) / 2

                Behavior on opacity { NumberAnimation { duration: 500 } }
            }

            InfoItem {
                id: name
                anchors.top: folderIcon.top
                anchors.left: folderIcon.right
                text: "<b>" + getName( filePath ) +"</b>"
                font.pointSize: 15
            }

            InfoItem {
                id: itemType
                anchors.top: name.bottom
                anchors.left: folderIcon.right
                text: folderStr
            }

            InfoItem {
                id: modified
                anchors.top: itemType.bottom
                anchors.left: folderIcon.right
                text: lastModifiedStr + " " + lastModified
            }

            InfoItem {
                id: size
                anchors.top: modified.bottom
                anchors.left: folderIcon.right
                text: sizeStr + " " + contentSize
            }

            InfoItem {
                id: content
                anchors.top: size.bottom
                anchors.left: folderIcon.right
                text: elementsStr + " " + countElements
            }

            Connections{
                target: photosListView;
                onCurrentIndexChanged: {
                    if ( listItem.ListView.isCurrentItem )
                    {
                        mainWindow.currentFileType = 4;
                        folderIcon.opacity = 1
                        mainWindow.updatePanel()
                    } else
                    {
                        folderIcon.opacity = 0
                    }
                }
            }
        }
    }

    Component {
        id: mimeDelegate
        Item {
            Image {
                id: mimeIcon
                source: "image://mime/" + filePath
                clip: true
                anchors.left: parent.left
                anchors.leftMargin: 20
                fillMode: Image.PreserveAspectFit
                asynchronous: true
                smooth: true;
                visible: albumWrapper.state === "fullscreen"
                height: getHeight( parent.height, 500 )
                y: ( parent.height - panel.height - height ) / 2

                Behavior on opacity { NumberAnimation { duration: 500 } }
            }

            InfoItem {
                id: name
                anchors.top: mimeIcon.top
                anchors.left: mimeIcon.right
                text: "<b>" + getName( filePath ) +"</b>"
                font.pointSize: 15
            }

            InfoItem {
                id: itemType
                anchors.top: name.bottom
                anchors.left: mimeIcon.right
                text: mime
            }

            InfoItem {
                id: modified
                anchors.top: itemType.bottom
                anchors.left: mimeIcon.right
                text: lastModifiedStr + " " + lastModified
            }

            InfoItem {
                id: size
                anchors.top: modified.bottom
                anchors.left: mimeIcon.right
                text: sizeStr + " " + contentSize
            }

            Connections{
                target: photosListView;
                onCurrentIndexChanged: {
                    if ( listItem.ListView.isCurrentItem )
                    {
                        mainWindow.currentFileType = 0;
                        mimeIcon.opacity = 1
                        mainWindow.updatePanel()
                    } else
                    {
                        mimeIcon.opacity = 0
                    }
                }
            }
        }
    }

    Component {
        id: okularDelegate

        Item {
            KPart {
                url: filePath
                width: parent.width
                height: parent.height
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
        else if ( t == 4 ) {
            fileModel.scanDirectory( index )
            return folderDelegate;
        }
        else if ( t == 5 )
            return audioDelegate;
        else if ( t == 6)
            return okularDelegate;
        else if(t == 0)
            return mimeDelegate;
    }

    Loader
    {
        id: componentLoader
        anchors.fill: parent;
        sourceComponent: bestDelegate( type )
    }
}

