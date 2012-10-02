import QtQuick 1.1
import Widgets 1.0
Component {
    id: progressDelegate
    Item {
        AnimatedImage {
            source: "images/loader.gif"
            anchors.centerIn: parent
        }
    }
}
