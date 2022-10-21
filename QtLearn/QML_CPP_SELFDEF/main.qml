import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import MyObj 1.0

Window {
    visible: true
    property int value: myobj.iValue
    width: 640
    height: 480
    title: qsTr("Hello World")

    MyObject {
        id: myobj
        iValue: 10
        sString: "zhangsan"

        Component.onCompleted: {
            console.log(iValue, sString);
        }
    }

    onValueChanged: {
        console.log("value", value);
    }

    Button {
        onClicked: {
            myobj.iValue += 20;
        }
    }
}
