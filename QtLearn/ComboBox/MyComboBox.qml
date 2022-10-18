import QtQuick 2.0
import QtQuick.Controls 2.5

ComboBox {
    id: com
    model: ["button", "checkBox", "popup"]
    Component.onCompleted: {
        console.log(count);
    }
}
