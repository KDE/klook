import QtQuick 1.1
import Widgets 1.0

Component {
    id: okularDelegate
    Item {
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
                    part.setPartParent()
                    part.url = filePath
                }
            }
            
        }
    }
}
