// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Text {
    wrapMode: Text.WordWrap
    color: ( mainWindow.state != "fullscreen" ) ? "black" : "white"
    elide: Text.ElideRight
    font.pointSize: 12
    anchors.right: parent.right
    anchors.leftMargin: 20
    anchors.rightMargin: 10
    anchors.topMargin: 10
    anchors.bottomMargin: 10
}
