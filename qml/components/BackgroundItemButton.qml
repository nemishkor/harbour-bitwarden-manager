import QtQuick 2.0
import Sailfish.Silica 1.0

BackgroundItem {
    id: button

    property string page
    property string label

    width: parent.width
    height: Theme.itemSizeMedium

    onClicked: pageStack.animatorPush(Qt.resolvedUrl(button.page))

    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: Theme.rgba(button.palette.highlightBackgroundColor, 0.1)
            }
            GradientStop {
                position: 1.0
                color: "transparent"
            }
        }
    }

    Label {
        text: label
        color: button.highlighted ? Theme.secondaryHighlightColor : Theme.highlightColor
        anchors {
            left: parent.left
            leftMargin: Theme.horizontalPageMargin
            right: parent.right
            rightMargin: icon.width + Theme.paddingSmall + Theme.horizontalPageMargin
            verticalCenter: parent.verticalCenter
        }
        font.pixelSize: Theme.fontSizeLarge
        truncationMode: TruncationMode.Fade
    }

    Icon {
        id: icon

        anchors {
            right: parent.right
            rightMargin: Theme.horizontalPageMargin
            verticalCenter: parent.verticalCenter
        }
        source: "image://theme/icon-m-right"
        highlighted: button.down
    }
}
