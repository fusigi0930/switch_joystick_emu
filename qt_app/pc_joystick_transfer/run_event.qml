import QtQuick 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Qt.labs.folderlistmodel 2.1

Dialog {
	id: runEventDialog
	modality: Qt.ApplicationModal

	signal sigRunEventList(variant list)

	Component.onCompleted: {

	}

    contentItem: Rectangle {
        anchors.fill: parent

        TableView {
            x: 15
            y: 15
            id: table
            width: parent.width - 30
            height: parent.height - buttonOK.height - 30 - 15
            model: listModel

            TableViewColumn{ role: "check"; title: ("v"); width: 25; delegate: CheckBox { onCheckedChanged: {
                    model.check = checked
                } } }
            TableViewColumn{ role: "fileName"; title: qsTr("file name"); width: 500 }

            FolderListModel {
                id: folderListModel
                folder: "file:."
                showDirs: false
                nameFilters: [ "*.bin" ]
            }

            ListModel {
                id: listModel
            }

            Repeater {
                model: folderListModel
                Component {
                    Item {
                        Component.onCompleted: {
                            var info
                            for (var i=0; i<folderListModel.count; i++) {
                                info = {"check": false,"fileName": folderListModel.get(i, "fileName")}
                                listModel.append(info)
                            }
                        }
                    }
                }
            }

        }

        Button {
            id: buttonCancel
            x: runEventDialog.width-width-25
            y: runEventDialog.height-buttonCancel.height-10
            text: qsTr("Cancel")

            onClicked: {
                close();
            }
        }

        Button {
            id: buttonOK
            x: buttonCancel.x-width-15
            y: runEventDialog.height-buttonOK.height-10
            text: qsTr("OK")
            onClicked: {
                var info = []
                for (var i = 0; i < listModel.count; i++) {
                    if (listModel.get(i)["check"] === true) {
                        info.push(listModel.get(i)["fileName"])
                    }
				}
                sigRunEventList(info)
                close();
            }
        }
    }
}
