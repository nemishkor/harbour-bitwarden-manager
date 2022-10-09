import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    onStatusChanged: {
        if (status == PageStatus.Active && !vaultManager.isLocked && syncService.synchronized) {
            foldersService.decryptAll()
        }
    }

    SilicaFlickable {
        visible: vaultManager.isLocked
        contentHeight: unlockColumn.height

        Column {
            id: unlockColumn
            width: page.width

            PageHeader {
                title: qsTr("Folders")
                description: "Bitwarden manager"
            }

            LockBlock {}
        }
    }

    SilicaFlickable {
        visible: !vaultManager.isLocked && !syncService.synchronized
        contentHeight: syncColumn.height

        Column {
            id: syncColumn
            width: page.width

            PageHeader {
                title: qsTr("Folders")
                description: "Bitwarden manager"
            }

            SyncBlock {}
        }
    }

    SilicaListView {
        visible: !vaultManager.isLocked && syncService.synchronized
        model: foldersListModel
        anchors {
            fill: parent
            bottomMargin: dockedPanel.visibleSize
        }
        clip: dockedPanel.expanded

        header: PageHeader {
            title: qsTr("Folders")
            description: "Bitwarden manager"
        }

        PullDownMenu {
            busy: foldersService.creating
            MenuItem {
                text: "Add folder"
                enabled: !foldersService.creating
                onClicked: pageStack.animatorPush(addFolderPage)
            }
        }

        delegate: ListItem {
            id: delegate
            contentHeight: column.height + separator.height + Theme.paddingMedium * 2
            onClicked: pageStack.animatorPush(Qt.resolvedUrl("Ciphers.qml"), { deleted: false, folderId: model.id })

            Column {
                id: column
                y: Theme.paddingMedium
                spacing: Theme.paddingSmall

                Label {
                    id: nameLabel
                    x: Theme.paddingLarge
                    text: model.name
                    color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
                }

                Label {
                    id: revisionDateLabel
                    visible: model.revisionDate !== ""
                    x: Theme.paddingLarge
                    text: qsTr("Last revision") + ": " + model.revisionDate
                    color: delegate.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                }
            }

            Separator {
                id: separator
                anchors {
                    top: column.bottom
                    topMargin: Theme.paddingMedium
                }

                width: parent.width
                color: Theme.primaryColor
                horizontalAlignment: Qt.AlignHCenter
            }
        }

        VerticalScrollDecorator {}
    }

    Component {
        id: addFolderPage

        Dialog {
            canAccept: folderNameInput.text.trim() !== ""
            onAccepted: {
                foldersService.create(folderNameInput.text)
            }

            Column {
                width: parent.width

                DialogHeader {
                    title: "Add folder"
                }

                TextField {
                    id: folderNameInput
                    focus: true
                    label: "Name"
                    placeholderText: label
                    width: parent.width
                }
            }

        }
    }

}
