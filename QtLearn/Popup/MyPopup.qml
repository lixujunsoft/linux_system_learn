import QtQuick 2.0
import QtQuick.Controls 2.5

Popup {
    id: popup
    x: 100
    y: 100
    width: 200
    height: 300
    visible: true

    // The default value is Popup.CloseOnEscape | Popup.CloseOnPressOutside
    // closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    closePolicy: Popup.NoAutoClose
    modal: true
    dim: true
    enter: Transition {
        NumberAnimation {
            property: "opacity"
            from: 0.0
            to: 1.0
            duration: 1000
        }
    }

    exit: Transition {
        NumberAnimation {
            property: "opacity"
            from: 1.0
            to: 0.0
            duration: 2000
        }
    }

    contentItem: Rectangle {
        anchors.fill: parent
        color: "grey"
        Text {
            id: txt
            anchors.fill: parent
            text: qsTr("Message Box Area!!! Message Box Area!!!")
            font.pixelSize: 26
            wrapMode: Text.WordWrap
        }

        Button {
            width: 80
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            anchors.right: parent.right
            anchors.rightMargin: 5
            text: qsTr("确认")
        }

        Button {
            width: 80
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 5
            text: qsTr("取消")
        }
    }

    // 设置除模态对话框以外的区域
    Overlay.modal: Rectangle {
        anchors.fill: parent
        // color: "lightsteelblue"
        // color: "#FF0000" // RGB 16进制
        color: "#33FF0000"  // RGBA --> ARGB

        Popup {
            width: parent.width
            height: 80
            closePolicy: Popup.NoAutoClose
            visible: true
            background: Rectangle {
                // color: "transparent"  // 透明度
                color: "grey"
            }
            Button {
                width: 80
                height: 50
                anchors.right: parent.right

                onClicked: {
                    console.log("Button OnClicked");
                }
            }
        }
    }

    // 设置除非模态对话框以外的区域
    Overlay.modeless: Rectangle {
        anchors.fill: parent
        color: "blue"
    }
}
