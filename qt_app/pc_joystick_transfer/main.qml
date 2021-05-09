import QtQuick.Window 2.12
import QtQuick 2.2
import QtQuick.Controls 2.0

import RpiJoyEmu 1.0

ApplicationWindow {
    visible: true
    width: screen.width * 2 /3
    height: 250
    title: "pc-rpi-joystick-emu transfer"
    objectName: "rpi-joystick-emu-main"

    onClosing: {
        console.log("closing")
    }

    Component.onCompleted: {
        console.log("start rpi joy emu class")
        rpi_joy_emu.init()
    }

    Rectangle {
        id: rectInfoArea
        x: 10
        y: 10
        width: parent.width - 20
        height: 35

        Text {
            id: labelRpiIp
            height: 28
            font.pixelSize: height - 5
            font.family: "tohama"
            text: "Raspberry pi joy emulator ip addr:"
        }
        Rectangle {
            anchors.left: labelRpiIp.right
            anchors.leftMargin: 5
            anchors.top: labelRpiIp.top
            anchors.topMargin: 1
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.bottom: labelRpiIp.bottom

            border.width: 1
            border.color: "#8080ff"
            visible: true

            TextInput {
                id: textRpiIp
                anchors.fill:parent
                font.pixelSize: 20
                font.family: "tohama"
                focus: true
                onTextChanged: rpi_joy_emu.setIp(text)
            }
        }
    }
    Rectangle {
        id: rectButtonArea
        width: rectInfoArea.width
        height: 35
        anchors.top: rectInfoArea.bottom
        anchors.topMargin: 5
        anchors.left: rectInfoArea.left

        Button {
            id: buttonStart
            x: 5
            y: 5
            width: 50
            height: 30
            text: "Start"
            onClicked: rpi_joy_emu.start()
        }

        Button {
            id: buttonStop
            x: buttonStart.width + buttonStart.x + 5
            y: 5
            width: 50
            height: 30
            text: "Stop"
            onClicked: rpi_joy_emu.stop()
        }
    }

    RpiJoyEmu {
        id: rpi_joy_emu
    }
}
