import QtQuick 2.15
import QtQuick.Window 2.15

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Text {
        id: txt
        color: "#00FF00"
        text: qsTr("green text\n 1234")
        Component.onCompleted: {
            console.log(contentWidth);
        }
    }

    Rectangle {
        y: 50
        width: 150
        height: 50
        border.color: "black"
        Text {
            id: text
            text: qsTr("text123455\n678912345567\n891234556789")
            elide: Text.ElideMiddle
            width: parent.width
            font.bold: true               // 粗体
            font.family: "Courier New"    // 字体
            font.italic: true             // 斜体
            font.letterSpacing: 10
            // font.pixelSize: 36         // 像素为单位
            font.pointSize: 36            // 磅为单位
            font.underline: true          // 下划线
            lineHeight: 1

            Component.onCompleted: {
                console.log("lineCount", lineCount);
                console.log("lineHeight", lineHeight);
            }
        }
    }

    Row {
        y: 210
        Text { font.pointSize: 24; text: "Normal" }
        Text { font.pointSize: 24; text: "Raised"; style: Text.Raised; styleColor: "#AAAAAA" }
        Text { font.pointSize: 24; text: "Outline";style: Text.Outline; styleColor: "red" }
        Text { font.pointSize: 24; text: "Sunken"; style: Text.Sunken; styleColor: "#AAAAAA" }
    }

    Column {
        y: 260
        Text {
            font.pointSize: 24
            text: "<b>Hello</b> <i>World!</i>"
        }
        Text {
            font.pointSize: 24
            textFormat: Text.RichText       // 富文本
            text: "<b>Hello</b> <i>World!</i>"
        }
        Text {
            font.pointSize: 24
            textFormat: Text.PlainText      // 纯文本
            text: "<b>Hello</b> <i>World!</i>"
        }
        Text {
            font.pointSize: 24
            textFormat: Text.MarkdownText   // Markdown
            text: "**Hello** *World!*"
        }
    }

    /* 文本换行 */
    Rectangle {
        y: 250
        x: 450
        id: rect
        width: 100
        height: 200
        border.color: "black"

        Text {
            id: myText
            text: qsTr("text text text text text text text")
            anchors.fill: parent
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
        }
    }

    /* 超链接 */
    Text {
        id: myTxt
        y: 420
        textFormat: Text.RichText
        text: "See the <a href=\"http://www.baidu.com\"> 百度</a>"
        onLinkActivated: {
            console.log(link + " link activated");
        }

        onLinkHovered: {
            console.log(link + "link hovered");
        }

        onHoveredLinkChanged: {
            console.log(link + "hovered link changed");
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
        }
    }
}
