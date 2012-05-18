import QtQuick 1.1
import Widgets 1.0

Component {
    id: okularDelegate
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

                    //t.forceActiveFocus()
                    part.setPartParent()
                }
            }
        }
    }
}
