import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page {
    id: page

    property string cipherId

    Component.onCompleted: cipherService.display(page.cipherId)

    function getTypeLabel(type){
        if(cipher.type === 1){
            return qsTr("Login");
        }
        if(cipher.type === 2){
            return qsTr("Secure note");
        }
        if(cipher.type === 3){
            return qsTr("Card");
        }
        if(cipher.type === 4){
            return qsTr("Identity");
        }
        return qsTr("Unknown type");
    }

    SilicaFlickable {
        visible: vaultManager.isLocked
        contentHeight: unlockColumn.height

        Column {
            id: unlockColumn
            width: page.width

            PageHeader {
                title: qsTr("Cipher")
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
                title: qsTr("Cipher")
            }
            SyncBlock {}
        }
    }

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {

        visible: !vaultManager.isLocked && syncService.synchronized

        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column

            width: page.width
            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTr("Cipher")
            }

            BackgroundItem {
                width: column.width
                height: contentItem.childrenRect.height
                Column {
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    Label { text: qsTr("Type"); color: Theme.highlightColor }
                    Label {
                        x: Theme.paddingLarge
                        text: getTypeLabel(cipher.type)
                        wrapMode: "WordWrap"
                    }
                }
            }

            BackgroundItem {
                width: column.width
                height: contentItem.childrenRect.height
                Column {
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    Label { text: qsTr("Name"); color: Theme.highlightColor }
                    Label {
                        x: Theme.paddingLarge
                        text: cipher.name
                        wrapMode: "WordWrap"
                    }
                }
            }

            BackgroundItem {
                visible: cipher.type === 1
                width: column.width
                height: contentItem.childrenRect.height
                Column {
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    Label { text: qsTr("Username"); color: Theme.highlightColor }
                    Label {
                        x: Theme.paddingLarge
                        text: cipher.loginUsername
                        wrapMode: "WordWrap"
                    }
                }
            }

        }

    }

}
