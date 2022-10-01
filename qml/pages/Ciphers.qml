import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page {
    id: page

    property bool deleted
    property string folderId

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    onStatusChanged: {
        if (status == PageStatus.Active && !vaultManager.isLocked && syncService.synchronized) {
            cipherService.decryptAll(deleted, folderId)
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
            contentHeight: column.height + separator.height
            onClicked: pageStack.animatorPush(Qt.resolvedUrl("Cipher.qml"), {cipherId: model.id})

            Icon {
                id: cipherIcon
                source: {
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
                y: (parent.height - cipherIcon.height) / 2
                anchors { left: parent.left; leftMargin: Theme.horizontalPageMargin }
            }

            Rectangle {
                id: column
                x: Theme.horizontalPageMargin + Theme.paddingMedium + Theme.iconSizeMedium
                width: parent.width - Theme.horizontalPageMargin - Theme.paddingMedium - Theme.iconSizeMedium
                height: topMarginRect.height + bottomMarginRect.height + cipherNameLabel.height * 2
                color: "transparent"

                Rectangle {
                    id: topMarginRect
                    anchors.top: parent.top
                    height: Theme.paddingMedium
                    width: parent.width
                    color: "transparent"
                }

                Label {
                    id: cipherNameLabel
                    anchors {
                        top: topMarginRect.bottom
                        topMargin: cipherSubtitleLabel.visible ? 0 : cipherNameLabel.height / 2
                    }

                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: model.name
                    color: Theme.primaryColor
                }

                Label {
                    id: cipherSubtitleLabel
                    anchors.top: cipherNameLabel.bottom
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    visible: model.subtitle !== ""
                    text: model.subtitle
                    color: Theme.secondaryColor
                }

                Rectangle {
                    id: bottomMarginRect
                    anchors.bottom: parent.bottom
                    height: Theme.paddingMedium
                    width: parent.width
                    color: "transparent"
                }

            }

            Separator {
                id: separator
                anchors.top: column.bottom
                width: parent.width
                color: Theme.primaryColor
                horizontalAlignment: Qt.AlignHCenter
            }
        }

        VerticalScrollDecorator {}

    }

}
