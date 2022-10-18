import QtQuick 2.0
import QtQuick.Controls 2.5

ListView {
    id: listView
    width: 180
    height: 200

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

    /*
    highlight: Rectangle {
        color: "lightsteelblue"
        radius: 5
    }

    delegate: Rectangle {
        color: "transparent"
        width: listView.width
        height: 50
        Text {
            id: name
            text: name + " " + Number + " " + myValue
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log(index);
                listView.currentIndex = index;
            }
        }
    }
    */

    // flickableDirection: Flickable.AutoFlickDirection  // 回弹效果
    flickableDirection: Flickable.HorizontalFlick        // 横向回弹
    delegate: Row {
        Text {
            text: "<b>Name:</b>" + name
            width: 160
        }

        Text {
            text: "<b>Number:</b>" + number
            width: 160
        }
    }
}
