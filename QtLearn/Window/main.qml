import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5

Window {
    width: 400
    height: 300
    visible: true
    title: qsTr("MY QML")

    minimumWidth: 400
    minimumHeight: 300
    maximumWidth: 600
    maximumHeight: 500

    Button {
        id: btn1
        width: 50
        height: 50

        focus: true
        background: Rectangle {
            border.color: btn1.focus ? "blue" : "black"
        }

        onClicked: {
            console.log("btn1 cliecked")
        }
    }

    Button {
        id: btn2
        x: 100
        width: 50
        height: 50
        background: Rectangle {
            border.color: btn2.focus ? "blue" : "black"
        }

        onClicked: {
            console.log("btn2 cliecked")
        }
    }

    onActiveFocusItemChanged: {
        console.log("active focus item changed", activeFocusItem)
    }
}
