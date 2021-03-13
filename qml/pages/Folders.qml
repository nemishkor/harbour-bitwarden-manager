import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaListView {

        model: foldersModel
        anchors.fill: parent
        header: PageHeader {
            title: qsTr("Folders")
        }
        delegate: ListItem {
            id: delegate
            contentHeight: column.height + separator.height + Theme.paddingMedium * 2

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
                    x: Theme.paddingLarge
                    text: model.revisionDate
                    color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
                }
                Label {
                    id: idLabel
                    x: Theme.paddingLarge
                    text: model.id
                    color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
                }
                Label {
                    id: userIdLabel
                    x: Theme.paddingLarge
                    text: model.userId
                    color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
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
