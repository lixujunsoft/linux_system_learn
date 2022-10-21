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
        anchors.left: parent.left
        anchors.rightMargin: 20

        Connections {
            target: loader1.item
            ignoreUnknownSignals: true

//            function onBtnSig(value) {
//                console.log("loader1", value);
//            }

            function onLeftBtnPressed(value) {
                loader2.item.focus = true;
                loader2.item.forceActiveFocus();
            }
        }

        Component.onCompleted: {
            loader1.item.focus = true;
            loader1.item.forceActiveFocus();
        }
    }

    Loader {
        id: loader2
        sourceComponent: com2
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20

        Connections {
            target: loader2.item
            ignoreUnknownSignals: true
            function onBtnSig(value) {
                console.log("loader2", value);
            }
        }
    }
}
