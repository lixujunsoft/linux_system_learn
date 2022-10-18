import QtQuick 2.0

import QtQuick.Controls 2.5

Rectangle {
    id: rect
    width: 400
    height: 200
    border.color: "black"
    anchors.centerIn: parent

    Component {
         id: sectionHeading
         Rectangle {
             width: rect.width
             height: childrenRect.height
             color: "lightsteelblue"

             Text {
                 text: section
                 font.bold: true
                 font.pixelSize: 20
             }
         }
     }

     ListView {
         id: view
         anchors.top: parent.top
         anchors.bottom: buttonBar.top
         width: parent.width
         height: parent.height

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

         delegate: Row { // 控制model的每一个数据如何绘制
             Text { text: "<b>Name:</b>" + name; width: 160 }
             Text { text: "<b>Number:</b>" + number; width: 160 }
         }

         section.property: "myValue"
         section.criteria: ViewSection.FullString
         section.delegate: sectionHeading  // 控制对应的section的property是如何绘制的
     }
}
