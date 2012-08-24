import QtQuick 1.1
import Widgets 1.0

Component {
    Item {
        id: t
        KPart {
            id: part
            url: filePath
            width: parent.width
            height: parent.height
        }
        Connections {
            target: photosListView
            onCurrentIndexChanged: {
                if ( listItem.ListView.isCurrentItem ) {
                    mouseControl.enabled = mainWindow.state == 'fullscreen' ? true : false
                    mainWindow.currentFileType = File.OkularFile
                    part.setPartParent()
                    mainWindow.updatePanel()
                }
                else
                    mouseControl.enabled = true
            }
        }
    }
}
