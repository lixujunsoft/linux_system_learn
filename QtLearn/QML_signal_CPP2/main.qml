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

    function qmlSlot(i, s) {  // 参数类型 对应c++端都是QVariant
        console.log("qml", i, s);
    }

    Button {
        objectName: "myButton"
        onClicked: {
            // myobj.cppSig(99, "lisi");
            MyObjectSingletonInstance.func();
        }
    }

    // 在QML端进行连接
//    Connections {
//        target: MyObjectSingletonInstance
//        function onCppSig(i, s) {
//            qmlSlot(i, s);
//        }
//    }
}
