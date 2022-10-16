import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Component {
        id: comButtonTest1
        Button {
            id: btn
            width: 50
            height: 50
            // flat: true
            // highlighted: true
            // checkable: true
        }
    }

    /*
    Button {
        id: btn1
        width: 50
        height: 50
        autoExclusive: true
        checkable: true
    }

    Button {
        id: btn2
        x: 60
        width: 50
        height: 50
        autoExclusive: true
        checkable: true
    }

    Button {
        id: btn3
        x: 120
        width: 50
        height: 50
        autoExclusive: true
        checkable: true
    }
    */

    Component {
        id: comButtonTest2
        Button {
            id: button
            x: 180
            width: 50
            height: 50
            autoRepeat: true
            autoRepeatDelay: 3000  // 延迟时间
            autoRepeatInterval: 1000 // 触发时间间隔

            onClicked: {
                console.log("onClicked");
            }

            onPressed: {
                console.log("onPressed");
            }

            onReleased: {
                console.log("onReleased");
            }
        }
    }

    Component {
        id: comButtonTest3
        Button {
            id: button
            width: 100
            height: 50
//            icon.source: "/cat.jpeg"
//            icon.height: 50
//            icon.width: 50
//            indicator: Image {
//                id: ind
//                source: "/cat.jpeg"
//                height: 50
//                width: 50
//            }
            text: "pressed"
        }
    }

    Component {
        id: comButtonTest4
        Button {
            id: button
            width: 100
            height: 50
            background: Rectangle {
                anchors.fill: button
                color: {
                    if (button.pressed) {
                        return "green";
                    } else {
                        return "blue";
                    }
                }

                border.width: 5
                border.color: {
                    if (button.pressed) {
                        return "red";
                    } else {
                        return "black";
                    }
                }
            }
        }
    }

    Loader {
        id: loader
        sourceComponent: comButtonTest4
    }
}
