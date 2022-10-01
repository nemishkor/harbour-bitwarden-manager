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
                    anchors.verticalCenter: parent.verticalCenter
                    x: Theme.horizontalPageMargin
                    source: vaultManager.isLocked ? "image://theme/icon-m-device-lock" : "image://theme/icon-m-keys"
                }
                Label {
                    anchors { left: cryptoIcon.right; leftMargin: Theme.paddingSmall; verticalCenter: cryptoIcon.verticalCenter }
                    text: vaultManager.isLocked ? qsTr("Locked") : qsTr("Unlocked")
                    color: highlighted ? Theme.highlightColor : Theme.primaryColor
                    width: page.width - 2 * Theme.horizontalPageMargin - cryptoIcon.width - Theme.paddingSmall
                }
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("Unlock.qml"))
            }

            BackgroundItem {
                width: column.width
                height: contentItem.childrenRect.height
                Column {
                    Label {
                        text: qsTr("Sync")
                        color: highlighted ? Theme.highlightColor : Theme.primaryColor
                        x: Theme.horizontalPageMargin
                    }
                    Label {
                        text: qsTr("Required")
                        visible: !syncService.synchronized
                        color: highlighted ? Theme.highlightColor : Theme.errorColor
                        x: Theme.horizontalPageMargin
                    }
                    Label {
                        text: qsTr("Last sync") + ": " + syncService.lastSync
                        visible: syncService.synchronized
                        color: highlighted ? Theme.highlightColor : Theme.primaryColor
                        x: Theme.horizontalPageMargin
                        font.pixelSize: Theme.fontSizeExtraSmall
                    }
                }
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("Sync.qml"))
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

            /*
            ExpandingSection {
                id: section

                title: "Lists"

                content.sourceComponent: Column {
                    width: section.width

                    BackgroundItem {
                        width: section.width
                        Label {
                            text: "Items"
                            color: highlighted ? Theme.highlightColor : Theme.primaryColor
                            anchors.verticalCenter: parent.verticalCenter
                            x: Theme.horizontalPageMargin
                        }
                        onClicked: pageStack.animatorPush(Qt.resolvedUrl("Items.qml"))
                    }
                    BackgroundItem {
                        width: section.width
                        Label {
                            text: "Folders"
                            color: highlighted ? Theme.highlightColor : Theme.primaryColor
                            anchors.verticalCenter: parent.verticalCenter
                            x: Theme.horizontalPageMargin
                        }
                    }
                    BackgroundItem {
                        width: section.width
                        Label {
                            text: "Collections"
                            color: highlighted ? Theme.highlightColor : Theme.primaryColor
                            anchors.verticalCenter: parent.verticalCenter
                            x: Theme.horizontalPageMargin
                        }
                    }
                    BackgroundItem {
                        width: section.width
                        Label {
                            text: "Organizations"
                            color: highlighted ? Theme.highlightColor : Theme.primaryColor
                            anchors.verticalCenter: parent.verticalCenter
                            x: Theme.horizontalPageMargin
                        }
                    }
                    BackgroundItem {
                        width: section.width
                        Label {
                            text: "Organizations collections"
                            color: highlighted ? Theme.highlightColor : Theme.primaryColor
                            anchors.verticalCenter: parent.verticalCenter
                            x: Theme.horizontalPageMargin
                        }
                    }
                    BackgroundItem {
                        width: section.width
                        Label {
                            text: "Organizations members"
                            color: highlighted ? Theme.highlightColor : Theme.primaryColor
                            anchors.verticalCenter: parent.verticalCenter
                            x: Theme.horizontalPageMargin
                        }
                    }
                }
            }
            */

            Button {
                text: "Logout"
                // see harbour-bitwarden-manager.qml
                onClicked: auth.logout()
                anchors.horizontalCenter: parent.horizontalCenter
            }

        }

    }

}
