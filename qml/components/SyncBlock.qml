import QtQuick 2.0
import Sailfish.Silica 1.0

Rectangle {
    id: root

    visible: !syncService.synchronized
    color: "black"
    height: col.height
    width: page.width

    Behavior on height {
        NumberAnimation {
            duration: 500
        }
    }

    Column {
        id: col
        spacing: Theme.paddingMedium
        width: parent.width

        Spacer {}

        Rectangle {
            id: syncIcon
            width: Theme.iconSizeMedium
            height: Theme.iconSizeMedium
            color: "transparent"
            anchors {
                horizontalCenter: parent.horizontalCenter
            }

            Image {
                visible: !syncService.isSyncing
                anchors.centerIn: parent
                source: "image://theme/icon-m-sync"
            }

            BusyIndicator {
                visible: syncService.isSyncing
                running: visible
                anchors.centerIn: parent
                size: BusyIndicatorSize.Medium
            }
        }

        Label {
            color: Theme.lightPrimaryColor
            text: syncService.isSyncing ? qsTr("Syncing...") : qsTr("Data is not synchronized")
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Button {
            visible: !syncService.isSyncing
            text: qsTr("Sync")
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                if(!syncService.isSyncing){
                    syncService.syncAll()
                }
            }
        }

        Spacer {}

    }
}
