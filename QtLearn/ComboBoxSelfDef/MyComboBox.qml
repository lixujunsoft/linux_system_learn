import QtQuick 2.15
import QtQuick.Controls 2.5

ComboBox {
    id: control
    height: 60
    width: 200
    model: ["First", "Second", "Third", "First", "Second", "Third", "First", "Second", "Third"]

    delegate: ItemDelegate {    // 针对model中每一项的具体绘制
        width: control.width
        contentItem: Text {
            text: modelData
            color: "#21be2b"
            font: control.font
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }
        highlighted: control.highlightedIndex === index

        required property int index
        required property var modelData
    }

    /*
    indicator: Image {
        id: img
        source: "/girl.jpeg"
        height: 50
        width: 50
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.verticalCenter: parent.verticalCenter
    }
    */

    indicator: Canvas {
        id: canvas
        x: control.width - width - control.rightPadding
        y: control.topPadding + (control.availableHeight - height) / 2
        width: 12
        height: 8
        contextType: "2d"

        Connections {
            target: control
            function onPressedChanged() { canvas.requestPaint(); }
        }

        onPaint: {
            context.reset();
            context.moveTo(0, 0);
            context.lineTo(width, 0);
            context.lineTo(width / 2, height);
            context.closePath();
            context.fillStyle = control.pressed ? "#17a81a" : "#21be2b";
            context.fill();
        }
    }

    contentItem: Text {
        leftPadding: 0
        rightPadding: control.indicator.width + control.spacing

        text: control.displayText
        font: control.font
        color: control.pressed ? "#17a81a" : "#21be2b"
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 120
        implicitHeight: 40
        border.color: control.pressed ? "#17a81a" : "#21be2b"
        border.width: control.visualFocus ? 2 : 1
        radius: 2
    }

    popup: Popup {
        y: control.height - 1
        width: control.width
        // implicitHeight: contentItem.implicitHeight
        implicitHeight: 150
        padding: 1

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex

            // ScrollIndicator.vertical: ScrollIndicator { }
            ScrollBar.vertical: ScrollBar {   // 实际项目中一般使用ScrollBar
                policy: ScrollBar.AlwaysOn
            }

            // ListView继承自Flickable，具有回弹的效果
            interactive: true  // 默认值为True, 如果想取消回弹效果，则可将其置为false
            boundsBehavior: Flickable.StopAtBounds
        }

        background: Rectangle { // 和delegate的区别是，delegate绘制单个项的，而popup绘制整个下拉控件
            border.color: "#21be2b"
            // border.color: "yellow"
            radius: 2
        }
    }
}
