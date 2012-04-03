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
                width: ( sourceSize.width > parent.width ) ? parent.width : sourceSize.width
                height: ( sourceSize.height > parent.height ) ? parent.height : sourceSize.height

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
                        panel.videoSlider.value = tick * 1000 / totalTime; // tick and totalTime in msec
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
        else if (t == 4)
            return folderDelegate;
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

