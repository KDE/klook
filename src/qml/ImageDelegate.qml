import QtQuick 1.1
import Widgets 1.0

Component {
    id: imgDelegate
    
    Item {
        id: imageItem
        
        Image {
            id: img
            opacity: 0
            anchors.centerIn: parent
            source: filePath
            fillMode: Image.PreserveAspectFit
            asynchronous: true
            smooth: true;
            visible: albumWrapper.state === "fullscreen"
            width: Math.min( sourceSize.width, parent.width )
            height: Math.min( sourceSize.height, parent.height )
            
            signal ready()
            
            onStatusChanged: if ( img.status === Image.Ready ) { ready(); opacity = 1; }
            
            Behavior on opacity { NumberAnimation { duration: 500 } }
        }
        
        Connections{
            target: photosListView;
            onCurrentIndexChanged: {
                if ( listItem.ListView.isCurrentItem )
                {
                    mainWindow.currentFileType = File.Image;
                    img.opacity = 1
                    mainWindow.updatePanel()
                } else
                {
                    img.opacity = 0
                }
            }
        }
    }
}
