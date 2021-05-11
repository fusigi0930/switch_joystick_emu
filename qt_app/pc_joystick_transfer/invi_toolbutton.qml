import QtQuick 2.0
import QtQuick.Controls 1.1

Item {
	id: rootItem
	property alias iconSource: icnBotton.iconSource
	property alias buttonText: icnBotton.text
	property alias labelText: label.text
	property alias tooltip: icnBotton.tooltip

	signal sigClicked()

	Rectangle {
		id: rectToolButton
		width:48
		height:40
		color: "transparent"
		ToolButton {
			id: icnBotton
			text: qsTr("add")
			iconSource: "/image/res/png/add.png"
			tooltip: qsTr("add a new script file to invisOX")
			anchors.horizontalCenter: parent.horizontalCenter
			width: parent.width * 0.8
			height: parent.width * 0.8

			onClicked: rootItem.sigClicked()

		}
		Text {
			id: label
			text: qsTr("add")
			anchors.top: icnBotton.bottom
			anchors.bottom: parent.bottom
			font.pixelSize: parent.height * 0.3
			font.family: "tohama"
			color: "#A0A0FF"
			anchors.horizontalCenter: parent.horizontalCenter
		}
	}
}
