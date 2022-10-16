import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Component {
        id: com1
        Button {
            width: 50
            height: 50
            background: Rectangle {
                anchors.fill: parent
                color: "red"
            }
        }
    }

    Component {
        id: com2
        Rectangle {
            width: 50
            height: 50
            color: "yellow"
        }
    }

    Component {
        id: com3
        Button {
            width: 50
            height: 50
        }
    }

    MyRectangle {
        width: 200
        height: 100
        myComponent: com3
        Component.onCompleted: {
            // rectWidth = 250;    // 不能给readonly修饰的属性赋值
            console.log(rectWidth);
            aliasInnerRectColor = "yellow";
        }
    }
}
