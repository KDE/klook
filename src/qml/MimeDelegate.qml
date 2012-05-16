import QtQuick 1.1
import Widgets 1.0

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
