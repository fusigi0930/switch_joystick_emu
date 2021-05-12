import QtQuick.Window 2.12
import QtQuick 2.2
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import "qrc:/modules/"

import RpiJoyEmu 1.0

ApplicationWindow {
    visible: true
    width: screen.width / 3
    height: 130
    title: "pc-rpi-joystick-emu transfer"
    objectName: "rpi-joystick-emu-main"

    onClosing: {
        console.log("closing")
    }

    Component.onCompleted: {
        console.log("start rpi joy emu class")
        rpi_joy_emu.init()
    }

    header: ToolBar {
            RowLayout {
            InviToolButton {
                id: buttonTransfer
                width: 56
                height: 56
                buttonText: "Start"
                labelText: "Start"
                iconSource: "image/res/png/gamepad.png"
                onSigClicked: {
                    if (iconSource == "qrc:/image/res/png/gamepad.png") {
                        rpi_joy_emu.start()
                        iconSource = "image/res/png/gamepad-stop.png"
                        buttonText = "Stop"
                        labelText = "Stop"
                        buttonRecord.enabled = true
                        buttonRunEvent.enabled = true
                    }
                    else {
                        rpi_joy_emu.stop()
                        rpi_joy_emu.setRecord(false)
                        rpi_joy_emu.stopRunEvent()
                        buttonRecord.enabled = false
                        buttonRunEvent.enabled = false
                        iconSource = "image/res/png/gamepad.png"
                        buttonText = "Start"
                        labelText = "Start"
                    }
                }
            }
            InviToolButton {
                id: buttonRecord
                enabled: false
                width: 56
                height: 56
                buttonText: "Record"
                labelText: "Record"
                iconSource: "image/res/png/record.png"
                onSigClicked: {
                    if (iconSource == "qrc:/image/res/png/record.png") {
                        rpi_joy_emu.setRecord(true)
                        iconSource = "image/res/png/record-stop.png"
                        buttonText = "Stop"
                        labelText = "Stop"
                    }
                    else {
                        rpi_joy_emu.setRecord(false)
                        iconSource = "image/res/png/record.png"
                        buttonText = "Record"
                        labelText = "Record"
                    }
                }
            }
            InviToolButton {
                id: buttonRunEvent
                enabled: false
                width: 56
                height: 56
                buttonText: "Run Event"
                labelText: "Run Event"
                iconSource: "image/res/png/run.png"
                onSigClicked: {
                    console.log("run event")
                    if (iconSource == "qrc:/image/res/png/run.png") {
                        rpi_joy_emu.runEvents("record.bin")
                        iconSource = "image/res/png/standing.png"
                        buttonText = "Terminate"
                        labelText = "Terminate"
                    }
                    else {
                        rpi_joy_emu.stopRunEvent()
                        iconSource = "image/res/png/run.png"
                        buttonText = "Run Event"
                        labelText = "Run Event"
                    }
                }
            }
        }
    }

    Rectangle {
        id: rectInfoArea
        x: 10
        y: 10
        width: parent.width - 20
        height: 20

        Text {
            id: labelRpiIp
            height: 28
            font.pixelSize: height - 5
            font.family: "tohama"
            text: "Rpi joy emu ip addr:"
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

            TextField {
                id: textRpiIp
                leftPadding: 10
                anchors.fill:parent
                font.pixelSize: 16
                font.family: "tohama"
                focus: true
                onTextChanged: rpi_joy_emu.setIp(text)
            }
        }
    }

    RpiJoyEmu {
        id: rpi_joy_emu
    }
}
