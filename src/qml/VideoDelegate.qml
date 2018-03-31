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
import Widgets 1.0

import QtMultimedia 5.8 as MultiMedia

Component {
    id: videoDelegate

    Item
    {
        id: videoItem
        signal ready()

        MultiMedia.Video {
            id: videoPlayer
            source: video.source
            width: metaData.resolution  ? metaData.resolution.width  : 640
            height: metaData.resolution ? metaData.resolution.height : 480

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    video.play()
                }
            }

            focus: true
            Keys.onSpacePressed: video.playbackState == MediaPlayer.PlayingState ? video.pause() : video.play()
            Keys.onLeftPressed: video.seek(video.position - 5000)
            Keys.onRightPressed: video.seek(video.position + 5000)

            visible: (albumWrapper.state === "") && (status !== MultiMedia.MediaPlayer.Loading) && (status !== MultiMedia.MediaPlayer.NoMedia)

            function doResize() {
                console.log(videoPlayer.width)
                video.setSizeHint(videoPlayer.width, videoPlayer.height);
            }

            onWidthChanged: {
                doResize();
            }

            onHeightChanged: {
                doResize();
            }

            Component.onCompleted: {
                doResize();
            }

        }

        Video {
            id: video
            source: filePath
            //anchors.fill: parent FIXME, porting

            Component.onCompleted: {
                source = filePath
                if ( albumWrapper.state === "" )
                    videoPlayer.play()
            }

            function setSizeHint(w, h)
            {
                if (index === photosListView.currentIndex) {
                    mainWidget.setVideoSizeHint(w, h, index)

                    // this is duplicated from SingleDelegate because video size can'be known
                    // until now
                    if(!mainWindow.firstFileLoaded && mainWindow.wrapperState !== 'inGrid') {
                        mainWidget.resizeToPreferredSize(photosListView.currentIndex)
                        mainWindow.firstFileLoaded = true
                    }
                }
            }
        }

        Connections
        {
            target: albumWrapper
            onStateChanged:
            {
                if ( albumWrapper.state === "inGrid" )
                    videoPlayer.pause()
                else
                    videoPlayer.play()
            }
        }

    }
}

