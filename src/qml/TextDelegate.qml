import QtQuick 1.1
import Widgets 1.0

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
                    mainWindow.currentFileType = File.Txt
                    mainWindow.updatePanel()
                    //Disable scrolling in embedded mode
                    if (mainWindow.state === 'embedded')
                        txt.setPreview(true)
                }
            }
        }
    }
}
