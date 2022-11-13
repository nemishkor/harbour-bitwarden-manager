import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page {
    id: page

    property bool deleted
    property string folderId

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    onStatusChanged: {
        if (status == PageStatus.Active) {
            decryptAllIfAllowed()
        }
    }

    function decryptAllIfAllowed(){
        if(!vaultManager.isLocked && syncService.synchronized){
            cipherService.decryptAll(deleted, folderId)
        }
    }

    SilicaFlickable {
        visible: vaultManager.isLocked || !syncService.synchronized
        contentHeight: syncLockColumn.height
        Column {
            id: syncLockColumn
            width: page.width
            PageHeader {
                title: qsTr("Ciphers")
                description: "Bitwarden manager"
            }
            LockBlock { id: lockBlock }
            Spacer { visible: lockBlock.visible }
            SyncBlock {}
        }
    }

    Connections {
        target: vaultManager
        onIsLockedChanged: {
            decryptAllIfAllowed()
        }
    }

    Connections {
        target: syncService
        onSynchronizedChanged: {
            decryptAllIfAllowed()
        }
    }

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaListView {
        id: listView

        visible: !vaultManager.isLocked && syncService.synchronized
        model: ciphersListModel
        anchors {
            fill: parent
            bottomMargin: selections.visibleSize
        }
        clip: selections.expanded
        header: PageHeader {
            id: pageHeader
            title: qsTr("Ciphers")
            description: "Bitwarden manager"
        }
        delegate: ListItem {
            id: delegate
            contentHeight: column.height + separator.height
            menu: contextMenu
            property int itemIndex: index
            property bool isChecked: model.checked
            function toggleCheck(){
                model.checked = !model.checked
            }

            Component {
                id: contextMenu
                ContextMenu {
                    MenuItem {
                        text: qsTr("Remove")
                        onClicked: ciphersListModel.remove(index)
                    }
                }
            }

            Icon {
                id: cipherIcon
                source: {
                    if(model.checked){
                        return "image://theme/icon-m-certificates";
                    }
                    if(ciphersListModel.checkedCount > 0){
                        return "image://theme/icon-m-tabs";
                    }
                    if(model.type === 1){
                        return "image://theme/icon-m-keys";
                    }
                    if(model.type === 2){
                        return "image://theme/icon-m-note";
                    }
                    if(model.type === 3){
                        return "image://theme/icon-m-file-vcard";
                    }
                    if(model.type === 4){
                        return "image://theme/icon-m-contact";
                    }
                    return "image://theme/icon-m-keys";
                }
                y: (parent.height - cipherIcon.height) / 2
                anchors { left: parent.left; leftMargin: Theme.horizontalPageMargin }
            }

            Rectangle {
                id: column
                x: Theme.horizontalPageMargin + Theme.paddingMedium + Theme.iconSizeMedium
                width: parent.width - Theme.horizontalPageMargin - Theme.paddingMedium - Theme.iconSizeMedium
                height: topMarginRect.height + bottomMarginRect.height + cipherNameLabel.height * 2
                color: "transparent"

                Rectangle {
                    id: topMarginRect
                    anchors.top: parent.top
                    height: Theme.paddingMedium
                    width: parent.width
                    color: "transparent"
                }

                Label {
                    id: cipherNameLabel
                    anchors {
                        top: topMarginRect.bottom
                        topMargin: cipherSubtitleLabel.visible ? 0 : cipherNameLabel.height / 2
                    }

                    width: parent.width - 2 * Theme.horizontalPageMargin
                    text: model.name
                    color: Theme.primaryColor
                }

                Label {
                    id: cipherSubtitleLabel
                    anchors.top: cipherNameLabel.bottom
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    visible: model.subtitle !== ""
                    text: model.subtitle
                    color: Theme.secondaryColor
                }

                Rectangle {
                    id: bottomMarginRect
                    anchors.bottom: parent.bottom
                    height: Theme.paddingMedium
                    width: parent.width
                    color: "transparent"
                }

            }

            Separator {
                id: separator
                visible: (index + 1) < listView.count
                anchors.top: column.bottom
                width: parent.width
                color: Theme.primaryColor
                horizontalAlignment: Qt.AlignHCenter
            }
        }

        VerticalScrollDecorator {}

    }

    MouseArea {
        z: 1
        visible: !vaultManager.isLocked && syncService.synchronized
        anchors {
            top: parent.top
            bottom: parent.bottom
            bottomMargin: selections.visibleSize
            left: parent.left;
            leftMargin: Theme.horizontalPageMargin
        }
        width: Theme.iconSizeMedium
        hoverEnabled: false

        /*
          First pressed list item check state
          if `true` than moving finger up or down marks items as checked
          if `false` than moving finger up or down marks items as unchecked
         */
        property bool checkMode: true

        function getYInList(mouseY){
            // header fix
            return Math.round(mouseY) + (listView.contentY < 0 ? listView.contentY : selections.height);
        }

        onPressed: {
            listView.interactive = false
            var item = listView.itemAt(mouseX, getYInList(mouseY))
            item.toggleCheck()
            checkMode = item.isChecked
        }
        onPositionChanged: {
            var item = listView.itemAt(mouseX, getYInList(mouseY))
            if((checkMode && item.isChecked) || (!checkMode && !item.isChecked)){
                return
            }
            item.toggleCheck()
        }
        onReleased: {
            listView.interactive = true
        }
        onCanceled: {
            listView.interactive = true
        }
    }

    DockedPanel {
        id: selections
        width: parent.width
        height: Theme.itemSizeExtraSmall * 2 + Theme.paddingLarge * 3
        open: ciphersListModel.checkedCount > 0
        ButtonLayout {
            anchors {
                verticalCenter: parent.verticalCenter
            }

            Button {
                preferredWidth: Theme.buttonWidthTiny
                text: qsTr("Select all")
                onClicked: ciphersListModel.selectAll()
            }
            Button {
                preferredWidth: Theme.buttonWidthTiny
                text: qsTr("Unselect all")
                onClicked: ciphersListModel.unselectAll()
            }
            Button {
                preferredWidth: Theme.buttonWidthTiny
                text: qsTr("Delete")
                onClicked: entitiesService.removeSelectedCiphers()
            }
        }
    }

}
