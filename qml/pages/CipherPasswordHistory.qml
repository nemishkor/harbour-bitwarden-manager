import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaListView {
        anchors.fill: parent
        model: cipherPasswordHistoryListModel
        header: PageHeader {
            title: qsTr("Password history")
            description: cipher.name
        }
        delegate: Item {
            id: delegate
            width: parent.width
            height: column.height + separator.height + Theme.paddingMedium * 2

            Column {
                id: column
                width: parent.width
                spacing: Theme.paddingMedium

                ListItem {
                    width: parent.width
                    contentHeight: lastUsageColumn.height
                    menu: ContextMenu {
                        MenuItem {
                            text: qsTr("Copy to clipboard")
                            onClicked: Clipboard.text = model.lastUsedDate
                        }
                    }
                    Column {
                        id: lastUsageColumn
                        x: Theme.horizontalPageMargin
                        width: parent.width - 2 * Theme.horizontalPageMargin
                        Label { text: qsTr("Last usage") }
                        Label {
                            text: model.lastUsedDate
                            font.pixelSize: Theme.fontSizeExtraSmall
                            wrapMode: "WordWrap"
                        }
                    }
                }

                ListItem {
                    width: parent.width
                    contentHeight: passwordColumn.height
                    menu: ContextMenu {
                        MenuItem {
                            text: qsTr("Copy to clipboard")
                            onClicked: Clipboard.text = model.password
                        }
                    }
                    Column {
                        id: passwordColumn
                        x: Theme.horizontalPageMargin
                        width: parent.width - 2 * Theme.horizontalPageMargin
                        Label { text: qsTr("Password") }
                        Label {
                            text: model.password
                            font.pixelSize: Theme.fontSizeExtraSmall
                            wrapMode: "WordWrap"
                        }
                    }
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
