import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {

        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column

            width: page.width
            spacing: Theme.paddingMedium

            PageHeader {
                title: user.email
                description: "Bitwarden manager"
            }

            BackgroundItem {
                width: column.width
                visible: !crypto.hasKey

                Image {
                    id: cryptoIcon
                    anchors {
                        left: parent.left
                        leftMargin: Theme.horizontalPageMargin
                        verticalCenter: parent.verticalCenter
                    }
                    source: vaultManager.isLocked ? "image://theme/icon-m-device-lock" : "image://theme/icon-m-keys"
                }

                Label {
                    anchors {
                        left: cryptoIcon.right
                        leftMargin: Theme.paddingSmall
                        verticalCenter: cryptoIcon.verticalCenter
                    }
                    text: vaultManager.isLocked ? qsTr("Locked") : qsTr("Unlocked")
                    color: highlighted ? Theme.highlightColor : Theme.primaryColor
                    width: page.width - 2 * Theme.horizontalPageMargin - cryptoIcon.width - Theme.paddingSmall
                }
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("Unlock.qml"))
            }

            BackgroundItem {
                width: column.width
                onClicked: {
                    if(!syncService.isSyncing){
                        console.log("Run sync all")
                        syncService.syncAll()
                    } else {
                        console.log("Do not run sync all: sync in progress")
                    }
                }

                Rectangle {
                    id: syncIcon
                    width: Theme.iconSizeMedium
                    height: Theme.iconSizeMedium
                    color: "transparent"
                    anchors {
                        left: parent.left
                        leftMargin: Theme.horizontalPageMargin
                        verticalCenter: parent.verticalCenter
                    }

                    Image {
                        visible: !syncService.isSyncing
                        anchors.verticalCenter: parent.verticalCenter
                        source: "image://theme/icon-m-sync"
                    }

                    BusyIndicator {
                        visible: syncService.isSyncing
                        running: visible
                        size: BusyIndicatorSize.Small
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }

                Column {
                    id: syncLabels
                    anchors {
                        left: syncIcon.right
                        leftMargin: Theme.paddingSmall
                        right: parent.right
                        rightMargin: Theme.horizontalPageMargin
                    }
                    spacing: 0

                    Label {
                        text: qsTr("Sync")
                        color: highlighted ? Theme.highlightColor : Theme.primaryColor
                    }
                    Label {
                        text: qsTr("Required")
                        visible: !syncService.synchronized
                        color: highlighted ? Theme.highlightColor : Theme.errorColor
                    }
                    Label {
                        text: qsTr("Last sync") + ": " + syncService.lastSync
                        visible: syncService.synchronized
                        color: highlighted ? Theme.highlightColor : Theme.primaryColor
                        font.pixelSize: Theme.fontSizeExtraSmall
                    }
                }
            }

            BackgroundItem {
                width: column.width
                Label {
                    text: qsTr("Profile")
                    color: highlighted ? Theme.highlightColor : Theme.primaryColor
                    anchors.verticalCenter: parent.verticalCenter
                    x: Theme.horizontalPageMargin
                }
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("Profile.qml"))
            }

            BackgroundItem {
                width: column.width
                Label {
                    text: qsTr("Folders") + " (" + vault.foldersCount + ")"
                    color: highlighted ? Theme.highlightColor : Theme.primaryColor
                    anchors.verticalCenter: parent.verticalCenter
                    x: Theme.horizontalPageMargin
                }
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("Folders.qml"))
            }

            BackgroundItem {
                width: column.width
                Label {
                    text: qsTr("All ciphers") + " (" + vault.ciphersCount + ")"
                    color: highlighted ? Theme.highlightColor : Theme.primaryColor
                    anchors.verticalCenter: parent.verticalCenter
                    x: Theme.horizontalPageMargin
                }
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("Ciphers.qml"), { deleted: false, folderId: null })
            }

            BackgroundItem {
                width: column.width
                Label {
                    text: qsTr("Trash") + " (" + vault.ciphersDeletedCount + ")"
                    color: highlighted ? Theme.highlightColor : Theme.primaryColor
                    anchors.verticalCenter: parent.verticalCenter
                    x: Theme.horizontalPageMargin
                }
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("Ciphers.qml"), { deleted: true })
            }

            Button {
                text: "Logout"
                // see harbour-bitwarden-manager.qml
                onClicked: auth.logout()
                enabled: tasks.count === 0
                anchors.horizontalCenter: parent.horizontalCenter
            }

        }

    }

}
