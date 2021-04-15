import QtQuick 2.0
import Sailfish.Silica 1.0

BackgroundItem {
    id: root

    property int cipherType
    property string label
    property string value

    visible: cipher.type === root.cipherType
    width: column.width
    height: contentItem.childrenRect.height
    Column {
        x: Theme.horizontalPageMargin
        width: parent.width - 2 * Theme.horizontalPageMargin
        Label { text: root.label }
        Label {
            text: root.value
            font.pixelSize: Theme.fontSizeExtraSmall
            wrapMode: "WordWrap"
        }
    }
}
