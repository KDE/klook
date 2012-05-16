import QtQuick 1.1
import Widgets 1.0

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
