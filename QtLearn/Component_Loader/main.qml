import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

//    Component.onCompleted: {
//        console.log("oncompleted", width, height, color);
//    }

//    Component.onDestruction: {
//        console.log("onDestruction");
//    }

//    Component {
//        id: com
//        Rectangle {
//            width: 200
//            height: 100
//            color: "black"

//            Component.onCompleted: {
//                console.log("oncompleted", width, height, color);
//            }

//            Component.onDestruction: {
//                console.log("onDestruction");
//            }
//        }
//    }

    Loader {
        id: loader
        sourceComponent: comAnimatedImage
        onStateChanged: {
            console.log("status: ", status);
        }
    }

//    Button {
//        width: 50
//        height: 50
//        x: 100
//        onClicked: {
//            //loader.sourceComponent = null;
//            loader.item.width = 50;
//            loader.item.height = 50;
//            loader.item.color = "red";
//        }
//    }

    Button {
        width: 50
        height: 50
        x: 100
        onClicked: {
            loader.item.paused = ! loader.item.paused
        }
    }

    Component {
        id: comImage
        Image {
            id: img
            source: "/girl.jpeg"
            sourceSize.height: 200
            sourceSize.width: 200
        }
    }

    Component {
        id: comAnimatedImage
        AnimatedImage {
            id: img
            source: "/BeatingHeart.gif"
            speed: 1
        }
    }
}
