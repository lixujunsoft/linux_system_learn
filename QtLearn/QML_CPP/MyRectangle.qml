import QtQuick 2.4
import QtQuick.Controls 2.5

Rectangle {
    // property int testValuel: 0
    width: 200
    height: 100
    color: "black"
    // 可通过别名方式将私有化的属性向外暴露(一般不用)
    property alias attr: attributes

    // 通过QtObject私有化属性
    QtObject {
        id: attributes
        property int testValue: 10
    }

    Component.onCompleted: {
        console.log(attributes.testValue);
    }
}
