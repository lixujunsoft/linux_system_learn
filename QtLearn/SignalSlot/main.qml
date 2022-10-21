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
            id: btn
            width: 100
            height: 50
            /*
            signal btnSig(int value)
            onClicked: {
                // console.log("123");
                btnSig(10);
            }
            */

            background: Rectangle {
                anchors.fill: parent
                border.color: btn.activeFocus ? "blue" : "black"
            }

            signal leftBtnPressed()
            Keys.onLeftPressed: {
                leftBtnPressed()
            }
        }
    }

    MyComponent {
        com1: com
        com2: com
    }
}
