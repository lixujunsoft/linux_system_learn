import QtQuick 2.0
import QtQuick.Controls 2.5

ComboBox {
    /*
    model: 3
    */

    textRole: "text"
    valueRole: "value"
    width: 300
    displayText: currentText + " " + currentValue


    // When an item is selected, update the backend.
    // onActivated: backend.modifier = currentIndex
    // Component.onCompleted: currentIndex = indexOfValue(backend.modifier)

    model: [
        { value: 100, text: qsTr("No modifier") },
        { value: 200, text: qsTr("Shift") },
        { value: 300, text: qsTr("Control") }
    ]

    onCurrentTextChanged: {
        console.log("text: ", currentText)
    }

    onCurrentValueChanged: {
        console.log("value: ", currentValue);
    }
}
