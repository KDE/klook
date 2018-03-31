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

Component {
    id: videoDelegate

    Item
    {
        id: videoItem
        signal ready()
        Video {
            id: video
            source: filePath
            //anchors.fill: parent FIXME, porting
            visible: (albumWrapper.state === "") && video.ready

            Component.onCompleted: {
                source = filePath
                if ( albumWrapper.state === "" )
                    video.play()
                if ( video.playing )
                    panel.playButtonState = 'Play'
                else
                    panel.playButtonState = 'Pause'

                video.sizeHintReady.connect(setSizeHint)

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

            onTicked:
            {
                if (playing) {
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
                    if ( video.playing ) {
                        video.pause()
                        panel.playButtonState = 'Play'
                    }
                    else {
                        video.play()
                        panel.playButtonState = 'Pause'
                    }
                }
            }
        }

        Connections{ target: panel.videoSlider; onPosChanged: video.setPosition( panel.videoSlider.value * video.totalTime / 1000 ) }
        Connections
        {
            target: albumWrapper
            onStateChanged:
            {
                if ( albumWrapper.state === "inGrid" )
                    video.pause()
                else
                    video.play()
            }
        }

    }
}

