import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    /*
    Column {  // 垂直布局
        id: col
        CheckBox {
            checked: true
            text: qsTr("First")
        }

        CheckBox {
            text: qsTr("Second")
        }

        CheckBox {
            text: qsTr("Third")
        }
    }

    // 通过ButtonGroup实现CheckBox的排他性
    ButtonGroup {
        id: childGroup
        exclusive: true

        buttons: col.children
    }
    */

    /*
    Column {  // 垂直布局
        id: col
        CheckBox {
            id: checkBox1
            checked: true
            text: qsTr("First")
            ButtonGroup.group: childGroup2
        }

        CheckBox {
            text: qsTr("Second")
            ButtonGroup.group: childGroup2
        }

        CheckBox {
            text: qsTr("Third")
            ButtonGroup.group: childGroup2
        }
    }

    ButtonGroup {
        id: childGroup2
        exclusive: false
        checkState: checkBox1.checkState
    }
    */

    Column {
        ButtonGroup {
            id: childGroup3
            exclusive: false
            checkState: parentBox.checkState
        }

        CheckBox {
            id: parentBox
            text: qsTr("parent")
            checkState: childGroup3.checkState
        }

        CheckBox {
            checked: true
            text: qsTr("Child 1")
            leftPadding: indicator.width
            ButtonGroup.group: childGroup3
        }

        CheckBox {
            text: qsTr("Child2")
            leftPadding: indicator.width
            ButtonGroup.group: childGroup3
        }
    }
}
