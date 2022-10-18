import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

//    Rectangle {
//        width: 200
//        height: 100
//        color: "black"
//        // visible: false
//        Rectangle {
//            width: 50
//            height: 50
//            color: "red"
//        }
//    }

//    Button {
//        width: 50
//        height: 50
//        onClicked: {
//            popup.close();
//        }
//    }

    /*
    Popup {  // 类似于Rectangle
        id: popup
        x: 300
        y: 100
        width: 200
        height: 300
        visible: true
        Component.onCompleted: {
            console.log("visible");
        }
    }

    // Popup 的两个例外
    // Popup 不会因为父控件的 visible状态而改变自己的 visible状态
    Rectangle {
        width: 200
        height: 100
        color: "black"
        visible: false
        Popup {
            width: 50
            height: 50
            visible: true
        }
    }

    // Popup 的z顺序是一个例外(Popup的z顺序只在Popup之间有效，Popup作为弹窗，默认遮盖其它控件)
    Popup {
        id: popup1
        z: 2
        y: 100
        width: 200
        height: 100
        visible: true
        background: Rectangle {
            color: "black"
        }
    }

    Popup {
        id: popup2
        z: 1
        y: 100
        width: 200
        height: 100
        visible: true
        background: Rectangle {
            color: "blue"
        }
    }
    */


    Button {
        width: 50
        height: 50
        onClicked: {
            myPopup.close();
        }
    }

    MyPopup {
        id: myPopup
    }
}
