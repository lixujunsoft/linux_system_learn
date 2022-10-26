import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5
import MyObj 1.0

Window {
    id: window
    width: 640
    height: 480
    visible: true
    objectName: "window"
    title: qsTr("Hello World")

    signal qmlSig(int i, string s)

    Button {
        objectName: "myButton"
        onClicked: {
            // myobj.func();
            // 发送信号, c++ MyObject端接收
            qmlSig(10, "zhangsan");
        }
    }

    MyObject {
        id: myobj
    }

// 在QML端完成信号和槽的绑定
//    Connections {
//        target: window
//        function onQmlSig(i, s) {
//            myobj.cppSlot(i, s);
//        }
//    }

//    Component.onCompleted: {
//        qmlSig.connect(myobj.cppSlot);
//    }
}
