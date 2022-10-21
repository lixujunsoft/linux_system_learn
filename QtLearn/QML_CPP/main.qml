import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5

Window {
    visible: true
    // width: SCREEN_WIDTH
    width: 640
    height: 480
    title: qsTr("Hello World")

    MyRectangle {
        Component.onCompleted: {
            attr.testValue = 200;
        }
    }
}
