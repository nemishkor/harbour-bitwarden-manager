import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page {
    id: page

    property bool deleted

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    onStatusChanged: {
        if (status == PageStatus.Active && !vaultManager.isLocked && syncService.synchronized) {
            cipherService.decryptAll(deleted)
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
                title: qsTr("Ciphers")
                description: "Bitwarden manager"
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
            description: "Bitwarden manager"
        }
        delegate: ListItem {
            id: delegate
            contentHeight: column.height + separator.height + Theme.paddingMedium * 2
            onClicked: pageStack.animatorPush(Qt.resolvedUrl("Cipher.qml"), {cipherId: model.id})

            Icon {
                source: {
                    console.log(model.type)
                    if(model.type === 1){
                        return "image://theme/icon-m-keys";
                    }
                    if(model.type === 2){
                        return "image://theme/icon-m-note";
                    }
                    if(model.type === 3){
                        return "image://theme/icon-m-file-vcard";
                    }
                    if(model.type === 4){
                        return "image://theme/icon-m-contact";
                    }
                    return "image://theme/icon-m-keys";
                }
                y: Theme.paddingMedium
                anchors { left: parent.left; leftMargin: Theme.horizontalPageMargin }
            }

            Column {
                id: column
                x: Theme.horizontalPageMargin + Theme.paddingMedium + Theme.iconSizeMedium
                y: Theme.paddingMedium
                width: parent.width - Theme.horizontalPageMargin - Theme.paddingMedium - Theme.iconSizeMedium

                Label {
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: model.name
                    color: Theme.primaryColor

                }
                Label {
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: model.subtitle
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
