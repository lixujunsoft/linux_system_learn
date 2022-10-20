import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5

Window {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    /*
    signal testSig(string s, int value)
    function func(ss, ii) {
        console.log(ss, ii);
    }

    Button {
        width: 50
        height: 50
        onClicked: {
            testSig("zhangsan", "99");
        }
    }

//    Component.onCompleted: {
//        testSig.connect(func);
//    }

    Connections {
        target: window
//        onTestSig: {
//            console.log(s, value);
//        }
        function onTestSig(str, value) {
            console.log(str, value);
        }
    }
    */

    Component {
        id: com
        Button {

        }
    }

    MyComponent {
        com1: com
        com2: com
    }
}
