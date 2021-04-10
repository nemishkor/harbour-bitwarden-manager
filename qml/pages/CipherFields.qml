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
        model: cipherFieldsListModel
        header: PageHeader {
            title: qsTr("Custom fields")
            description: cipher.name
        }
        delegate: BackgroundItem {
            width: column.width
            height: contentItem.childrenRect.height + Theme.paddingMedium
            Column {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                TextSwitch {
                    visible: model.type === 2
                    text: model.name
                    checked: model.value === "true"
                }
                Label {
                    visible: model.type !== 2
                    id: label
                    text: model.name
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    wrapMode: Text.Wrap
                }
                Label {
                    visible: model.type !== 2
                    id: desc
                    text: {
                        if(model.type === 1){
                            var str = "";
                            var chars = Math.max(model.value.length, 8);
                            for(var i = 0; i < chars; i++){
                                str += "•";
                            }
                            return str;
                        }
                        return model.value;
                    }
                    width: label.width
                    wrapMode: Text.Wrap
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                }
            }
        }
        VerticalScrollDecorator {}

    }

}
