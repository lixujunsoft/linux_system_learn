import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5

Window {
    id: window
    width: 640
    height: 480
    visible: true
    title: qsTr("MouseArea")
    color: "white"

//    MouseArea {
//        id: mouseArea
//        width: 200
//        height: 200
//        hoverEnabled: true
//        acceptedButtons: Qt.LeftButton | Qt.RightButton
//        cursorShape: Qt.CrossCursor
//        Rectangle {
//            anchors.fill: parent
//            color: "black"
//        }

//        onClicked: {
//            console.log("clicked");
//        }

//        onPressed: {
//            var ret = pressedButtons & Qt.LeftButton;
//            var ret2 = pressedButtons & Qt.RightButton;
//            console.log(ret ? "left" : (ret2 ? "right" : "other"));
//            // console.log("pressed");
//        }

//        onContainsMouseChanged: {
//            console.log("onContainsMouseChanged", containsMouse);
//        }

//        onContainsPressChanged: {
//            console.log("onContainsPressChanged", containsPress);
//        }

//        onHoveredChanged: {
//            console.log("onHoveredChanged");
//        }
//    }

//    Rectangle {
//        id: rect

//        width: 50; height: 50
//        color: "red"
//        // opacity: (600.0 - rect.x) / 600

//        MouseArea {
//            anchors.fill: parent
//            drag.target: rect
//            drag.axis: Drag.XAxis | Drag.YAxis
//            drag.minimumX: 0
//            drag.minimumY: 0
//            drag.maximumX: window.width - rect.width
//            drag.maximumY: window.height - rect.height
//        }
//    }

//    Rectangle {
//         width: 480
//         height: 320
//         color: "black"
//         Rectangle {
//             x: 30; y: 30
//             width: 300; height: 240
//             color: "lightsteelblue"

//             MouseArea {
//                 anchors.fill: parent
//                 drag.target: parent;
//                 drag.axis: "XAxis"
//                 drag.minimumX: 30
//                 drag.maximumX: 150
//                 drag.filterChildren: true

//                 Rectangle {
//                     color: "yellow"
//                     x: 50; y : 50
//                     width: 100; height: 100
//                     MouseArea {
//                         anchors.fill: parent
//                         onClicked: console.log("Clicked")
//                     }
//                 }
//             }
//         }
//     }

//    Rectangle {
//        width: 480
//        height: 320
//        color: "black"
//        MouseArea {
//            anchors.fill: parent
//            pressAndHoldInterval: 3000
//            onPressAndHold: {
//                console.log("onPressAndHold");
//            }
//        }
//    }

    Rectangle {
         color: "yellow"
         width: 100; height: 100

         MouseArea {
             anchors.fill: parent
             onClicked: console.log("clicked yellow")
         }

         Rectangle {
             color: "blue"
             width: 50; height: 50

             MouseArea {
                 anchors.fill: parent
                 propagateComposedEvents: true
                 onClicked: (mouse)=> {
                     console.log("clicked blue")
                     mouse.accepted = false
                 }
             }
         }
     }
}
