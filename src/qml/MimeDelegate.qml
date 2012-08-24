import QtQuick 1.1
import Widgets 1.0

Component {
    Item {
        Image {
            id: mimeIcon
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

            return w
        }

        InfoItem {
            id: name
            anchors.left: mimeIcon.right
            text: "<b>" + getName(filePath) +"</b>"
            font.pointSize: 15
            y: ( mimeIcon.height - mimeIcon.paintedHeight ) / 2 + iconHeightMargin
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
            elide: Text.ElideNone
            wrapMode: Text.NoWrap
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
                    mainWindow.currentFileType = File.Undefined;
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
