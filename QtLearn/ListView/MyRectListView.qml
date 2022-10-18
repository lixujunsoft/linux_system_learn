import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle {
    id: rect
    width: 400
    height: 200
    border.color: "black"
    anchors.centerIn: parent

    ListView {
        id: listView
        width: rect.width
        height: rect.height

        header: Rectangle {
            width: rect.width
            height: 20
            color: "blue"
        }

        footer: Rectangle {
            width: rect.width
            height: 20
            color: "green"
        }

        model: ListModel {
            ListElement {
                name: "Bill Smith"
                number: "555 3246"
                myValue: 111
            }

            ListElement {
                name: "John Brown"
                number: "555 8426"
                myValue: 222
            }

            ListElement {
                name: "Sam Wise"
                number: "555 0437"
                myValue: 333
            }
        }

        spacing: 20
        delegate: Row {
            Text {
                text: "<b>Name:</b>" + name
                width: 160
            }

            Text {
                text: "<b>Number:</b>" + number + " " + size
                width: 160
            }
        }
    }
}
