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
