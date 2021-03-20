import QtQuick 2.0
import Sailfish.Silica 1.0

Rectangle {

    id: root

    property string text: ""

    visible: vaultManager.isLocked
    color: "black"
    anchors.horizontalCenter: parent.horizontalCenter
    height: cryptoIcon.height + label.height + button.height + Theme.paddingMedium * 2 + Theme.paddingLarge * 2
    width: page.width
    Image {
        id: cryptoIcon
        source: "image://theme/icon-m-device-lock"
        anchors { horizontalCenter: parent.horizontalCenter; top: parent.top; topMargin: Theme.paddingLarge }
    }
    Label {
        id: label
        color: Theme.lightPrimaryColor
        text: qsTr("Vault is locked") + (root.text === "" ? "" : (". " + root.text))
        anchors { horizontalCenter: parent.horizontalCenter; top: cryptoIcon.bottom; topMargin: Theme.paddingMedium }
    }
    Button {
        id: button
        text: qsTr("Unlock")
        onClicked: pageStack.animatorPush(Qt.resolvedUrl("../pages/Unlock.qml"))
        anchors { horizontalCenter: parent.horizontalCenter; top: label.bottom; topMargin: Theme.paddingMedium }
    }
}
