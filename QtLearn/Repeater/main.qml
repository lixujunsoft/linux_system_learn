import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    /*
    Button {
        width: 50
        height: 50
    }

    Button {
        x: 60
        width: 50
        height: 50
    }

    Button {
        x: 120
        width: 50
        height: 50
    }
    */

    /*
    // model 使用数字类型
    Repeater {
        model: 3 // 模型 数字：表示了有几个模型/控件
        Rectangle {
            y: index * 50
            width: 100
            height: 40
            border.width: 1
            color: "yellow"
        }
    }
    */

    // model 使用list类型
    Repeater {
        model: ["Button", "Rectangle", "MouseArea", "CheckBox"]
        Button {
            y: index * 50
            width: 100
            height: 40
            text: modelData + " " +index
        }

        Component.onCompleted: {
            console.log(count);
        }
    }
}
