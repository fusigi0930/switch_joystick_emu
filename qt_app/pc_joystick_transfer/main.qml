import QtQuick.Window 2.12
import QtQuick 2.2
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.1
import "qrc:/modules/"

import RpiJoyEmu 1.0

ApplicationWindow {
    id: mainWindow
    visible: true
    width: screen.width / 3
    height: 130
    title: "pc-rpi-joystick-emu transfer"
    objectName: "rpi-joystick-emu-main"

    Loader {
        id: runEventDialog
    }

    onClosing: {
        console.log("closing")
        rpi_joy_emu.stop()
    }

    Component.onCompleted: {
        console.log("start rpi joy emu class")
        rpi_joy_emu.init()
    }

    FileDialog {
        id: saveRecordDialog
        folder: "file:."
        fileMode: FileDialog.SaveFile
        onAccepted: {
            if (currentFile === "")
                return
            console.log("filename: ", file)
            rpi_joy_emu.setRecordFileName(file)
            rpi_joy_emu.setRecord(true)
            buttonRecord.iconSource = "image/res/png/record-stop.png"
            buttonRecord.buttonText = "Stop"
            buttonRecord.labelText = "Stop"
        }
    }

    header: ToolBar {
            RowLayout {
            InviToolButton {
                id: buttonTransfer
                width: 56
                height: 56
                buttonText: "Connect"
                labelText: "Connect"
                iconSource: "image/res/png/gamepad.png"
                onSigClicked: {
                    if (iconSource == "qrc:/image/res/png/gamepad.png") {
                        rpi_joy_emu.start()
                        iconSource = "image/res/png/gamepad-stop.png"
                        buttonText = "Disconnect"
                        labelText = "Disconnect"
                        buttonRecord.enabled = true
                        buttonRunEvent.enabled = true
                    }
                    else {
                        rpi_joy_emu.stop()
                        rpi_joy_emu.setRecord(false)
                        rpi_joy_emu.stopRunEvent()
                        resetToolButtons()
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
                        saveRecordDialog.visible = true
                        console.log("after dialog")
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
                    if (iconSource == "qrc:/image/res/png/run.png") {
                        var dlg = runEventDialog.setSource("qrc:/modules/run_event.qml", {
                                        visible: true,
                                        title: "Run Event",
                                        width: mainWindow.width,
                                        height: mainWindow.height*3,
                                        focus: false
                                        } )
                        //rpi_joy_emu.runEvents("record.bin")
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

    function resetToolButtons() {
        buttonTransfer.enabled = true
        buttonRecord.enabled = false
        buttonRunEvent.enabled = false
        buttonTransfer.iconSource = "image/res/png/gamepad.png"
        buttonTransfer.buttonText = "Connect"
        buttonTransfer.labelText = "Connect"

        buttonRunEvent.iconSource = "image/res/png/run.png"
        buttonRunEvent.buttonText = "Run Event"
        buttonRunEvent.labelText = "Run Event"

        buttonRecord.iconSource = "image/res/png/record.png"
        buttonRecord.buttonText = "Record"
        buttonRecord.labelText = "Record"
    }
}
