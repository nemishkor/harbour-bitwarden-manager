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

            y: Theme.paddingLarge
            width: parent.width
            spacing: Theme.paddingLarge

            Icon {
                id: icon
                anchors.horizontalCenter: parent.horizontalCenter
                source: "image://theme/icon-m-vpn"
                color: Theme.highlightColor
            }

            Label {
                color: Theme.highlightColor
                text: "Bitwarden"
                width: parent.width - 2 * Theme.horizontalPageMargin
                x: Theme.horizontalPageMargin
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Theme.fontSizeExtraLarge
            }

            Label {
                color: Theme.highlightColor
                text: "Open Source Password Manager"
                width: parent.width - 2 * Theme.horizontalPageMargin
                x: Theme.horizontalPageMargin
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Theme.fontSizeLarge
            }

            Label {
                color: Theme.highlightColor
                text: "Warning!\nThe application at alpha state and can be unsecure. Some credentials stores as plain text to sync database. These credentials can not be used to decrypt data. Use this app if you trust your device only"
                width: parent.width - 2 * Theme.horizontalPageMargin
                x: Theme.horizontalPageMargin
                horizontalAlignment: Text.AlignHCenter
                wrapMode: "WordWrap"
            }

            Button {
                text: qsTr("Login")
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("Login.qml"))
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Button {
                text: qsTr("Register")
                onClicked: Qt.openUrlExternally("https://vault.bitwarden.com/#/register")
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Separator {
                width: parent.width
                color: Theme.primaryColor
                horizontalAlignment: Qt.AlignHCenter
            }

            Label {
                color: Theme.highlightColor
                text: "Be free to leave your propositions or report bug on Github"
                width: parent.width - 2 * Theme.horizontalPageMargin
                x: Theme.horizontalPageMargin
                horizontalAlignment: Text.AlignHCenter
                wrapMode: "WordWrap"
            }

            ButtonLayout {

                Button {
                    text: qsTr("Github")
                    onClicked: Qt.openUrlExternally("https://github.com/nemishkor/harbour-bitwarden-manager/issues")
                }

                Button {
                    text: qsTr("OpenRepos")
                    onClicked: Qt.openUrlExternally("https://openrepos.net/content/nemishkor/bitwarden-manager-alpha")
                }

            }

        }

        VerticalScrollDecorator {}

    }

    onStatusChanged: {
        if (status == PageStatus.Active && auth.loginStage === 4) { // if authorized already
            pageStack.animatorReplace(Qt.resolvedUrl("Home.qml"), {}, PageStackAction.Immediate)
        }
    }

}
