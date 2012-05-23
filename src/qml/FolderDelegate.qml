import QtQuick 1.1
import Widgets 1.0

Component {
    id: folderDelegate
    
    Item {
        //        id: folderItem

        Image {
            id: folderIcon
            anchors.left: parent.left
            anchors.leftMargin: leftItemMargin
            anchors.top: parent.top
            anchors.topMargin: iconHeightMargin
            anchors.bottom: parent.bottom
            anchors.bottomMargin: iconHeightMargin + panel.height
            source: "image://mime/" + filePath
            clip: true
            fillMode: Image.PreserveAspectFit
            asynchronous: true
            smooth: true;
            visible: albumWrapper.state === "fullscreen"
            width: getIconWidth( paintedWidth, paintedHeight, getMaxTextWidth() )

            Behavior on opacity { NumberAnimation { duration: 500 } }
        }

        function getMaxTextWidth()
        {
            var w = name.paintedWidth

            if ( w < itemType.paintedWidth )
                w = itemType.paintedWidth
            if ( w < modified.paintedWidth )
                w = modified.paintedWidth
            if ( w < size.paintedWidth )
                w = size.paintedWidth
            if ( w < content.paintedWidth )
                w = content.paintedWidth

            return w
        }

        InfoItem {
            id: name
            text: "<b>" + getName( filePath ) +"</b>"
            anchors.left: folderIcon.right
            font.pointSize: 15
            y: ( folderIcon.height - folderIcon.paintedHeight ) / 2 + iconHeightMargin
        }

        InfoItem {
            id: itemType
            anchors.top: name.bottom
            anchors.left: folderIcon.right
            text: folderStr
        }

        InfoItem {
            id: modified
            anchors.left: folderIcon.right
            anchors.top: itemType.bottom
            text: lastModifiedStr + " " + lastModified
            wrapMode: Text.NoWrap
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
                if ( listItem.ListView.isCurrentItem ) {
                    mainWindow.currentFileType = 4;
                    folderIcon.opacity = 1
                    mainWindow.updatePanel()
                } else {
                    folderIcon.opacity = 0
                }
            }
        }
    }
}
