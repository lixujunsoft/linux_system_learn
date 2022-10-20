
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Button {
        id: btn1
        width: 100
        height: 50
        focus: true

        background: Rectangle {
            anchors.fill: parent
            border.color: btn1.activeFocus ? "blue" : "balck"
        }

        onFocusChanged: {
            console.log("focus:", focus)
        }

        Component.onCompleted: {
            console.log("focusPolicy", focusPolicy);
        }
    }

    Button {
        id: btn2
        y: 100
        width: 100
        height: 50
        focus: true

        background: Rectangle {
            anchors.fill: parent
            border.color: btn2.activeFocus ? "blue" : "balck"
        }

        onFocusChanged: {
            console.log("focus:", focus)
        }

        Component.onCompleted: {
            console.log("focusPolicy", focusPolicy);
        }
    }
}
