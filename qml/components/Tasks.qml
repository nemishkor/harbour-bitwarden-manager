import QtQuick 2.0
import Sailfish.Silica 1.0

DockedPanel {
    id: root

    property bool maximized: false
    property real maximizedHeight: Math.min(root.parent.height / 3, Theme.dp(400))

    function toggleHeight(){
        root.maximized = !root.maximized
        root.height = root.maximized ? maximizedHeight : (Theme.itemSizeExtraLarge + Theme.paddingLarge)
    }

    height: tasks.count > 0 ? (Theme.itemSizeExtraLarge + Theme.paddingLarge) : 0
    width: parent.width
    open: tasks.count > 0
    dock: Dock.Bottom
    clip: true

    Behavior on height {
        NumberAnimation {
            duration: 500
            easing.type: Easing.OutQuad
        }
    }

    ListView {
        id: list

        anchors {
            top: parent.top
            topMargin: Theme.paddingMedium
            bottom: parent.bottom
            bottomMargin: Theme.paddingMedium
        }
        width: parent.width
        height: parent.height - Theme.paddingLarge
        model: tasks
        spacing: 0

        function iconNameByStatusCode(status){
            if(status === -1)
                return "icon-s-high-importance"
            if(status === 1)
                return "icon-s-checkmark"
            return "icon-s-maybe"
        }

        delegate: ListItem {
            id: listItem
            height: label.height
            contentHeight: label.height
            width: parent.width

            Rectangle {
                id: icon
                width: Theme.iconSizeSmall
                height: Theme.iconSizeSmall
                color: "transparent"

                anchors {
                    top: parent.top
                    topMargin: (label.height - Theme.iconSizeSmall) / 2
                    left: parent.left
                    leftMargin: Theme.horizontalPageMargin
                }

                BusyIndicator {
                    visible: model.status === 0
                    running: visible
                    size: BusyIndicatorSize.ExtraSmall
                    anchors.verticalCenter: parent.verticalCenter
                }

                Icon {
                    visible: model.status !== 0
                    source: {
                        var source = "image://theme/"
                                + list.iconNameByStatusCode(model.status)
                                + "?"
                                + (model.status === -1
                                   ? Theme.errorColor
                                   : (listItem.highlighted ? Theme.highlightColor : Theme.primaryColor))
                        return source
                    }
                }
            }

            Label {
                id: label

                anchors {
                    left: icon.right
                    leftMargin: Theme.paddingSmall
                    right: parent.right
                    rightMargin: Theme.horizontalPageMargin
                }
                padding: Theme.paddingSmall
                text: model.name + (model.message ? (": " + model.message) : "")
                color: {
                    if(model.status === -1){
                        return Theme.errorColor
                    }

                    return listItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                }
                wrapMode: "WrapAnywhere"
            }
        }
    }

}
