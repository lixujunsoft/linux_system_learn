import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5

Window {
    width: 1000
    height: 800
    visible: true
    title: qsTr("Hello World")

    /*
    Rectangle {
        x: 100
        y: 100
        z: 0
        focus: true
        width: 100
        height: 50
        color: "blue"

        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log("on clicked")
            }
        }

        Keys.onReturnPressed: {
            console.log("on return pressed")
        }
    }
    */

    Rectangle {
        id: rect1
        width: 100
        height: 50
        color: "black"
    }

    Rectangle {
        id: rect2
        width: 100
        height: 50
        anchors.left: rect1.right
        anchors.leftMargin: 20
        color: "black"
    }

    Rectangle {
        id: rect3
        width: 100
        height: 50
        border.width: 10
        border.color: "red"
        color: "black"
        radius: 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        gradient: Gradient {
            GradientStop { position: 0.0; color: "lightsteelblue" }
            GradientStop { position: 1.0; color: "blue" }
        }
    }

    Rectangle {
        y: 100; width: 80; height: 80
        color: "lightsteelblue"
    }

    MyRectangle {
        y: 200
        width: 200
        height: 100
        myTopMargin: 10
        myBottomMargin: 10
        myLeftMargin: 5
        myRightMargin: 5
    }

    /* 状态机 */
    Rectangle {
        id: root
        width: 100; height: 100
        color: "black"
        state: "normal"
        states: [ /* 状态机的使用 */
            State {
                name: "red_color"
                PropertyChanges { target: root; color: "red"; width: 200}
            },
            State {
                name: "blue_color"
                PropertyChanges { target: root; color: "blue"; height: 200}
            }
        ]

        MouseArea {
            anchors.fill: parent
            onPressed: {
                root.state = "red_color"
            }

            onReleased: {
                root.state = "blue_color"
            }
        }
    }

    /* 动画 */
    Rectangle {
        id: flashingblob
        width: 75; height: 75
        y: 400
        color: "blue"
        opacity: 0.1

        MouseArea {
            anchors.fill: parent
            onClicked: {
                animateColor.start()
                animateOpacity.start()
                animateWidth.start()
            }
        }

        PropertyAnimation {
            id: animateColor;
            target: flashingblob;
            properties: "color";
            to: "green";
            duration: 2000
        }

        NumberAnimation {
            id: animateOpacity
            target: flashingblob
            properties: "opacity"
            from: 0.1
            to: 1.0
            duration: 2000
       }


        NumberAnimation {
            id: animateWidth
            target: flashingblob
            property: "width"
            from: 75
            to: 150
            duration: 2000
        }
    }

    Rectangle {
        id: rect
        y: 500
        width: 100; height: 100
        color: "red"

        PropertyAnimation on x {
            to: 100
            duration: 1000
        }

        PropertyAnimation on y {
            to: 100
            duration: 1000
        }

        PropertyAnimation on width {
            to: 300
            duration: 3000
        }

        PropertyAnimation on color {
            to: "black"
            duration: 3000
        }
    }

    /* 顺序动画 */
    Rectangle {
         width: 100; height: 100
         y: 500
         color: "red"

         SequentialAnimation on color {
             ColorAnimation {
                 to: "yellow";
                 duration: 2000
             }

             ColorAnimation {
                 to: "blue";
                 duration: 2000
             }
         }
    }

    /* 状态机动画 */
    Rectangle {
        y: 600
        width: 75; height: 75
        id: button
        state: "RELEASED"

        MouseArea {
            anchors.fill: parent
            onPressed: button.state = "PRESSED"
            onReleased: button.state = "RELEASED"
        }

        states: [
            State {
                name: "PRESSED"
                PropertyChanges { target: button; color: "lightblue"}
            },
            State {
                name: "RELEASED"
                PropertyChanges { target: button; color: "lightsteelblue"}
            }
        ]

        transitions: [
            Transition {
                from: "PRESSED"
                to: "RELEASED"
                ColorAnimation { target: button; duration: 1000}
            },
            Transition {
                from: "RELEASED"
                to: "PRESSED"
                ColorAnimation { target: button; duration: 1000}
            }
        ]
    }

    /* 预先设置行为的动画 */
    Rectangle {
        y: 700
        width: 75; height: 75; radius: width
        id: ball
        color: "salmon"

        component BounceAnimation : NumberAnimation {
           easing {
              type: Easing.OutElastic
              amplitude: 1.0
              period: 0.5
           }
        }

        Behavior on x {
            BounceAnimation {}
        }
        Behavior on y {
            BounceAnimation {}
        }
        Behavior on color {
            ColorAnimation { target: ball; duration: 100 }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                ball.x += 50
                ball.y += 40
            }
        }
    }

    Rectangle {
        id: banner
        x: 500
        y: 700
        width: 150; height: 100; border.color: "black"

        Column {
            anchors.centerIn: parent
            Text {
                id: code
                text: "Code less."
                opacity: 0.01
            }
            Text {
                id: create
                text: "Create more."
                opacity: 0.01
            }
            Text {
                id: deploy
                text: "Deploy everywhere."
                opacity: 0.01
            }
        }

        MouseArea {
            anchors.fill: parent
            onPressed: playbanner.start()
        }

        SequentialAnimation {
            id: playbanner
            running: false
            NumberAnimation { target: code; property: "opacity"; to: 1.0; duration: 200}
            NumberAnimation { target: create; property: "opacity"; to: 1.0; duration: 200}
            NumberAnimation { target: deploy; property: "opacity"; to: 1.0; duration: 200}
        }
    }

}
