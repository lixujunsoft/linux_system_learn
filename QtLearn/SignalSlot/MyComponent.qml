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
        anchors.bott
    }

    Loader {
        id: loader2
        sourceComponent: com2
    }
}
