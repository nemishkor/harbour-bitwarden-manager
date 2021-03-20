import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    onStatusChanged: {
        if (status == PageStatus.Active && !vaultManager.isLocked && syncService.synchronized) {
            cipherService.decryptAll()
        }
    }

    SilicaFlickable {
        visible: vaultManager.isLocked
        contentHeight: unlockColumn.height

        Column {
            id: unlockColumn
            width: page.width

            PageHeader {
                title: qsTr("Ciphers")
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
                title: qsTr("Ciphers")
            }
            SyncBlock {}
        }
    }

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaListView {

        visible: !vaultManager.isLocked && syncService.synchronized
        model: ciphersListModel
        anchors.fill: parent
        header: PageHeader {
            title: qsTr("Ciphers")
        }
        delegate: ListItem {
            id: delegate
            contentHeight: column.height + separator.height + Theme.paddingMedium * 2

            Column {
                id: column
                y: Theme.paddingMedium

                Label {
                    x: Theme.paddingLarge
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: model.name
                    color: Theme.primaryColor
                }
                Label {
                    x: Theme.paddingLarge
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: model.username
                    color: Theme.secondaryColor
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

}
