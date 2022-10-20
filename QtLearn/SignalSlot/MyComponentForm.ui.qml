import QtQuick 2.4
import QtQuick.Controls 2.5

Rectangle {
    width: 400
    height: 300
    property Component com1
    property Component com2

    Loader {
        id: loader1
        sourceComponent: com1
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
    }

    Loader {
        id: loader2
        sourceComponent: com2
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 120
    }
}
