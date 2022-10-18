import QtQuick 2.0
import QtQuick.Controls 2.5

ComboBox {
    editable: true
    model: 10
    validator: IntValidator {bottom: 0; top: 10;}
    focus: true
    onAcceptableInputChanged: {
        console.log(acceptableInput);
    }
}
