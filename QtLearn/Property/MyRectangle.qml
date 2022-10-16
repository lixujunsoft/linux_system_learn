import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle {
    id: borderRect
    property int myTopMargin: 0
    property int myBottomMargin: 0
    property int myLeftMargin: 0
    property int myRightMargin: 0
    property string myString: "black"
    property color myColor: "black"
    property url myUrl: "qrc:/cat.jpeg"

    /* 要求外部必须得为属性赋值 */
    required property Component myComponent

    /* 定义外部只能访问、不能修改的属性 */
    readonly property int rectWidth: width;

    /* 通过别名向外界暴露子控件 */
    property alias aliasInnerRect: innerRect

    /* 通过别名向外界暴露子控件属性 */
    property alias aliasInnerRectColor: innerRect.color

    color: "black"

    Rectangle {
        id: innerRect
        color: myColor
        anchors.fill: parent
        anchors.topMargin: myTopMargin
        anchors.bottomMargin: myBottomMargin
        anchors.leftMargin: myLeftMargin
        anchors.rightMargin: myRightMargin

        Loader {
            id: loader
            sourceComponent: myComponent
        }
    }
}
