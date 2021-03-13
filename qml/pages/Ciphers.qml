import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaListView {

        visible: crypto.hasKey
        model: ciphersModel
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
                    text: "id:"
                    color: Theme.primaryColor
                }
                Label {
                    x: 2 * Theme.paddingLarge
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: model.id
                    color: Theme.primaryColor
                }

                Label {
                    x: Theme.paddingLarge
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: "organization id:"
                    color: Theme.primaryColor
                }
                Label {
                    x: 2 * Theme.paddingLarge
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: model.organizationId
                    color: Theme.primaryColor
                }

                Label {
                    x: Theme.paddingLarge
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: "folder id:"
                    color: Theme.primaryColor
                }
                Label {
                    x: 2 * Theme.paddingLarge
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: model.folderId
                    color: Theme.primaryColor
                }

                Label {
                    x: Theme.paddingLarge
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: "user id:"
                    color: Theme.primaryColor
                }
                Label {
                    x: 2 * Theme.paddingLarge
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: model.userId
                    color: Theme.primaryColor
                }

                Label {
                    x: Theme.paddingLarge
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: "edit:"
                    color: Theme.primaryColor
                }
                Label {
                    x: 2 * Theme.paddingLarge
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: model.edit ? "+" : "-"
                    color: Theme.primaryColor
                }

                Label {
                    x: Theme.paddingLarge
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: "view password:"
                    color: Theme.primaryColor
                }
                Label {
                    x: 2 * Theme.paddingLarge
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: model.viewPassword ? "+" : "-"
                    color: Theme.primaryColor
                }

                Label {
                    x: Theme.paddingLarge
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: "organization use totp:"
                    color: Theme.primaryColor
                }
                Label {
                    x: 2 * Theme.paddingLarge
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: model.organizationUseTotp
                    color: Theme.primaryColor
                }

                Label {
                    x: Theme.paddingLarge
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: "favorite:"
                    color: Theme.primaryColor
                }
                Label {
                    x: 2 * Theme.paddingLarge
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: model.favorite ? "+" : "-"
                    color: Theme.primaryColor
                }

                Label {
                    x: Theme.paddingLarge
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: "revision date:"
                    color: Theme.primaryColor
                }
                Label {
                    x: 2 * Theme.paddingLarge
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: model.revisionDate
                    color: Theme.primaryColor
                }

                Label {
                    x: Theme.paddingLarge
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: "size name:"
                    color: Theme.primaryColor
                }
                Label {
                    x: 2 * Theme.paddingLarge
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: model.sizeName
                    color: Theme.primaryColor
                }

                Label {
                    x: Theme.paddingLarge
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: "name:"
                    color: Theme.primaryColor
                }
                Label {
                    x: 2 * Theme.paddingLarge
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: model.name
                    color: Theme.primaryColor
                }

                Label {
                    x: Theme.paddingLarge
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: "notes:"
                    color: Theme.primaryColor
                }
                Label {
                    x: 2 * Theme.paddingLarge
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: model.notes
                    color: Theme.primaryColor
                }

                Label {
                    x: Theme.paddingLarge
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: "deleted date:"
                    color: Theme.primaryColor
                }
                Label {
                    x: 2 * Theme.paddingLarge
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: model.deletedDate
                    color: Theme.primaryColor
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

    Image {
        anchors.centerIn: parent
        visible: !crypto.hasKey
        source: "image://theme/icon-m-device-lock"
    }

}
