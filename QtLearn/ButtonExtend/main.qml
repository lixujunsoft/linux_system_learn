import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    DelayButton {
        width: 150
        height: 50
        delay: 3000
        onProgressChanged: {
            console.log(progress);
        }
    }

    Column {
        id: col
        y: 60
        RadioButton {
            checked: true
            text: qsTr("First")
        }

        RadioButton {
            text: qsTr("Second")
        }

        RadioButton {
            text: qsTr("Third")
        }
    }

    Column {
        id: colSwitch
        y: 180

        Switch {
            text: qsTr("Wi-Fi")
            LayoutMirroring.enabled: true
            onPositionChanged: {
                console.log("pos: ", position);
            }
            onVisualPositionChanged: {
                console.log("vis pos: ", visualPosition);
            }
        }

        Switch {
            text: qsTr("Bluetooth")
        }
    }

    /* 通过ButtonGroup实现Switch的自动排他 */
    /*
    ButtonGroup {
        id: btngrp
        exclusive: true
        buttons: colSwitch.children
    }
    */

    TabBar {
        y: 280
        TabButton {
            text: qsTr("Home")
        }

        TabButton {
            text: qsTr("Disc")
        }

        TabButton {
            text: qsTr("Acti")
        }
    }

    RoundButton {
        y: 300
        text: "test"
    }

    Button {
        y: 360
        x: 100
        id: btn
        width: 150
        height: 100
        padding: 0
        text: qsTr("Button")
//        background: Rectangle {
//            anchors.fill: parent
//            color: btn.checked | btn.down ? "blue" : "black"
//        }

//        contentItem: Label {
//            text: btn.text
//            font: btn.font
//            verticalAlignment: Text.AlignVCenter
//            horizontalAlignment: Text.AlignHCenter
//        }

//        contentItem: Text {
//            text: btn.text
//            font.pixelSize: 18
//            font.bold: true
//        }

        contentItem: Rectangle {
            id: rect
            Image {
                id: image
                width: btn.width
                height: btn.height
                source: "/girl1.jpeg"
            }

            Text {
                id: text
                text: btn.text
                anchors.centerIn: rect
            }
        }

        onClicked: {
            console.log("onClicked");
        }
    }
}
