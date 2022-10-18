import QtQuick 2.0
import QtQuick.Controls 2.5

ComboBox {
    editable: true
    model: ListModel {
        id: model
        ListElement { text: "Banana" }
        ListElement { text: "Apple" }
        ListElement { text: "Coconut" }
    }
    onAccepted: {
        if (find(editText) === -1) {
            model.append({text: editText});
        }
    }
}
