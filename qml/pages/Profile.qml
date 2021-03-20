import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page {
    id: page

    backNavigation: !syncService.isSyncing

    Component.onCompleted: {
        if(!syncService.isSyncing && !syncService.synchronized){
            syncService.syncAll()
        }
    }

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

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
            spacing: Theme.paddingLarge

            PageHeader {
                title: qsTr("Cipher")
            }



        }

    }

}
