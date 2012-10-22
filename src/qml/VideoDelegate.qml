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

Component {
    id: videoDelegate

    Item
    {
        id: videoItem
        signal ready()
        Video {
            id: video
            opacity: 0
            source: filePath
            anchors.fill: parent
            visible: albumWrapper.state === 'fullscreen' && video.ready

            Component.onCompleted: {
                video.opacity = 1
                video.setPlayerParent()
                video.source = filePath
                mainWindow.currentFileType = File.Video
                mainWindow.updatePanel()
                if ( albumWrapper.state === "fullscreen" )
                    video.play()

                if ( video.playing )
                    panel.playButtonState = 'Play'
                else
                    panel.playButtonState = 'Pause'
            }

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
