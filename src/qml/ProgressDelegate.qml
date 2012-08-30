import QtQuick 1.1
import Widgets 1.0
Component {
        id: progressDelegate

        ProgressItem {
            Component.onCompleted: {
                setJobWidget()
            }

        }
}
