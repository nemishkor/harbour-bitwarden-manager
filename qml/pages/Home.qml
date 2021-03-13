import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

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
                title: user.email
            }

            BackgroundItem {
                width: section.width
                Label {
                    text: qsTr("Sync")
                    color: highlighted ? Theme.highlightColor : Theme.primaryColor
                    anchors.verticalCenter: parent.verticalCenter
                    x: Theme.horizontalPageMargin
                }
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("Sync.qml"))
            }

            BackgroundItem {
                width: section.width
                Label {
                    text: qsTr("Profile")
                    color: highlighted ? Theme.highlightColor : Theme.primaryColor
                    anchors.verticalCenter: parent.verticalCenter
                    x: Theme.horizontalPageMargin
                }
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("Profile.qml"))
            }

            BackgroundItem {
                width: section.width
                Label {
                    text: qsTr("Folders")
                    color: highlighted ? Theme.highlightColor : Theme.primaryColor
                    anchors.verticalCenter: parent.verticalCenter
                    x: Theme.horizontalPageMargin
                }
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("Folders.qml"))
            }

            ExpandingSection {
                id: section

                title: "Lists"

                content.sourceComponent: Column {
                    width: section.width

                    BackgroundItem {
                        width: section.width
                        Label {
                            text: "Items"
                            color: highlighted ? Theme.highlightColor : Theme.primaryColor
                            anchors.verticalCenter: parent.verticalCenter
                            x: Theme.horizontalPageMargin
                        }
                        onClicked: pageStack.animatorPush(Qt.resolvedUrl("Items.qml"))
                    }
                    BackgroundItem {
                        width: section.width
                        Label {
                            text: "Folders"
                            color: highlighted ? Theme.highlightColor : Theme.primaryColor
                            anchors.verticalCenter: parent.verticalCenter
                            x: Theme.horizontalPageMargin
                        }
                    }
                    BackgroundItem {
                        width: section.width
                        Label {
                            text: "Collections"
                            color: highlighted ? Theme.highlightColor : Theme.primaryColor
                            anchors.verticalCenter: parent.verticalCenter
                            x: Theme.horizontalPageMargin
                        }
                    }
                    BackgroundItem {
                        width: section.width
                        Label {
                            text: "Organizations"
                            color: highlighted ? Theme.highlightColor : Theme.primaryColor
                            anchors.verticalCenter: parent.verticalCenter
                            x: Theme.horizontalPageMargin
                        }
                    }
                    BackgroundItem {
                        width: section.width
                        Label {
                            text: "Organizations collections"
                            color: highlighted ? Theme.highlightColor : Theme.primaryColor
                            anchors.verticalCenter: parent.verticalCenter
                            x: Theme.horizontalPageMargin
                        }
                    }
                    BackgroundItem {
                        width: section.width
                        Label {
                            text: "Organizations members"
                            color: highlighted ? Theme.highlightColor : Theme.primaryColor
                            anchors.verticalCenter: parent.verticalCenter
                            x: Theme.horizontalPageMargin
                        }
                    }
                }
            }

        }

    }

}
