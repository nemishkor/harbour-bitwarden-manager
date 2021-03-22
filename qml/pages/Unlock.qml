import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column

            width: page.width
            spacing: Theme.paddingLarge

            PageHeader {
                title: qsTr("Lock/unlock")
                description: "Bitwarden manager"
            }

            Label {
                text: qsTr("Unlocked vault means that application has key to decrypt data. Master password required to generate key only")
                width: parent.width - 2 * Theme.horizontalPageMargin
                x: Theme.horizontalPageMargin
                wrapMode: "WordWrap"
                color: Theme.highlightColor
            }

            PasswordField {
                id: passwordField
                visible: vaultManager.isLocked
                readOnly: vaultManager.unlocking
                label: qsTr("Master password")
                width: parent.width
                validator: RegExpValidator { regExp: /^.+$/ }
                showEchoModeToggle: true
                EnterKey.enabled: acceptableInput
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: { vaultManager.unlock(passwordField.text); passwordField.text = "" }
            }

            ButtonLayout {
                Button {
                    preferredWidth: Theme.buttonWidthMedium
                    enabled: vaultManager.isLocked && passwordField.acceptableInput && !vaultManager.unlocking
                    text: qsTr("Unlock vault")
                    onClicked: { vaultManager.unlock(passwordField.text); passwordField.text = "" }
                }
                Button {
                    preferredWidth: Theme.buttonWidthMedium
                    enabled: !vaultManager.isLocked
                    text: qsTr("Lock vault")
                    onClicked: vaultManager.lock()
                }
            }

            Label {
                text: vaultManager.unlockMessage
                visible: vaultManager.isLocked && vaultManager.unlockMessage != ""
                width: parent.width - 2 * Theme.horizontalPageMargin
                x: Theme.horizontalPageMargin
                wrapMode: "WordWrap"
                color: Theme.highlightColor
            }

            ProgressCircle {
                id: circle
                visible: vaultManager.unlocking
                anchors.horizontalCenter: parent.horizontalCenter
                NumberAnimation on value {
                    from: 0
                    to: 1
                    duration: 1000
                    running: vaultManager.unlocking
                    loops: Animation.Infinite
                }
            }

            Label {
                width: parent.width
                text: vaultManager.isLocked ? qsTr("Vault is locked") : qsTr("Vault is unlocked")
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Theme.fontSizeHuge
                color: Theme.highlightColor
            }

        }

        VerticalScrollDecorator {}

    }

}
