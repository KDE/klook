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
    id: audioDelegate

    Item
    {
        id: audioItem

        signal ready()

        Image {
            id: audioIcon
            anchors.left: parent.left
            anchors.leftMargin: leftItemMargin
            anchors.top: parent.top
            anchors.topMargin: iconHeightMargin
            anchors.bottom: parent.bottom
            anchors.bottomMargin: iconHeightMargin + panel.height
            source: "images/audio.png"
            clip: true
            opacity: 0
            fillMode: Image.PreserveAspectFit
            asynchronous: true
            smooth: true;
            visible: albumWrapper.state === "fullscreen"
            width: getIconWidth( paintedWidth, paintedHeight, getMaxTextWidth() )

            Behavior on opacity { NumberAnimation { duration: 500 } }

        }

        // getting of maximum width of text info
        function getMaxTextWidth()
        {
            var w = title.paintedWidth

            if ( w < artist.paintedWidth ) {
                w = artist.paintedWidth
            }
            if ( w < totalTime.paintedWidth ) {
                w = totalTime.paintedWidth
            }

            return w
        }

        InfoItem {
            id : title
            font.pointSize: 15
            anchors.left: audioIcon.right
            y: ( audioIcon.height - audioIcon.paintedHeight ) / 2 + iconHeightMargin
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
            wrapMode: Text.NoWrap
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
