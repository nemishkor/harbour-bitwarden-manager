import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    backNavigation: !syncService.isSyncing

    Component.onCompleted: {
        syncService.syncAll()
    }

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
            spacing: Theme.paddingLarge

            PageHeader {
                title: qsTr("Sync")
            }

            ProgressCircle {
                id: circle
                visible: syncService.isSyncing
                anchors.horizontalCenter: parent.horizontalCenter
                NumberAnimation on value {
                    from: 0
                    to: 1
                    duration: 1000
                    running: syncService.isSyncing
                    loops: Animation.Infinite
                }
            }

            Label {
                visible: syncService.message !== ""
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                text: syncService.message
                color: syncService.messageType === "error" ? Theme.errorColor : Theme.primaryColor
                wrapMode: "WordWrap"
            }

        }

    }

}
