import QtQuick 2.0
import Sailfish.Silica 1.0
import "../components"

Page {
    id: page

    property string cipherId

    Component.onCompleted: cipherService.display(page.cipherId)

    function getTypeLabel(type){
        if(cipher.type === 1){
            return qsTr("Login");
        }
        if(cipher.type === 2){
            return qsTr("Secure note");
        }
        if(cipher.type === 3){
            return qsTr("Card");
        }
        if(cipher.type === 4){
            return qsTr("Identity");
        }
        return qsTr("Unknown type");
    }

    SilicaFlickable {
        visible: vaultManager.isLocked
        contentHeight: unlockColumn.height

        Column {
            id: unlockColumn
            width: page.width

            PageHeader {
                title: qsTr("Cipher")
                description: "Bitwarden manager"
            }

            LockBlock {}
        }
    }

    SilicaFlickable {
        visible: !vaultManager.isLocked && !syncService.synchronized
        contentHeight: syncColumn.height
        Column {
            id: syncColumn
            width: page.width
            PageHeader {
                title: qsTr("Cipher")
                description: "Bitwarden manager"
            }
            SyncBlock {}
        }
    }

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {

        visible: !vaultManager.isLocked && syncService.synchronized

        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column

            width: page.width
            spacing: Theme.paddingMedium

            PageHeader {
                title: cipher.name
                description: "Bitwarden manager"
            }

            Item {
                x: Theme.horizontalPageMargin
                height: Theme.iconSizeMedium
                width: parent.width - 2 * Theme.horizontalPageMargin

                Icon {
                    id: icon
                    source: {
                        if(cipher.type === 1){
                            return "image://theme/icon-m-keys";
                        }
                        if(cipher.type === 2){
                            return "image://theme/icon-m-note";
                        }
                        if(cipher.type === 3){
                            return "image://theme/icon-m-file-vcard";
                        }
                        if(cipher.type === 4){
                            return "image://theme/icon-m-contact";
                        }
                        return "image://theme/icon-m-keys";
                    }
                }
                Label {
                    height: Theme.iconSizeMedium
                    x: Theme.iconSizeMedium + Theme.paddingSmall
                    color: Theme.primaryColor
                    verticalAlignment: Text.AlignVCenter
                    text: {
                        if(cipher.type === 1){
                            return "Login";
                        }
                        if(cipher.type === 2){
                            return "Secure note";
                        }
                        if(cipher.type === 3){
                            return "Card";
                        }
                        if(cipher.type === 4){
                            return "Personal data";
                        }
                        return "Other";
                    }
                }
            }

            Item {
                x: Theme.horizontalPageMargin
                height: Theme.iconSizeMedium
                width: parent.width - 2 * Theme.horizontalPageMargin

                Icon {
                    source: "image://theme/icon-m-favorite" + (cipher.favorite ? "-selected" : "")
                }
                Label {
                    x: Theme.iconSizeMedium + Theme.paddingSmall
                    height: Theme.iconSizeMedium
                    color: Theme.primaryColor
                    verticalAlignment: Text.AlignVCenter
                    text: cipher.favorite ? qsTr("Is favorite") : qsTr("Isn't favorite")
                }
            }

            SectionHeader { text: qsTr("Notes") }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                text: cipher.notes
                wrapMode: "WordWrap"
            }

            SectionHeader { text: qsTr("Login"); visible: cipher.type === 1 }

            BackgroundItem {
                visible: cipher.type === 1
                width: column.width
                height: contentItem.childrenRect.height
                Column {
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    Label { text: qsTr("Username"); color: Theme.highlightColor }
                    Label {
                        x: Theme.paddingLarge
                        text: cipher.loginUsername
                        wrapMode: "WordWrap"
                    }
                }
            }

            BackgroundItem {
                visible: cipher.type === 1
                width: column.width
                height: contentItem.childrenRect.height
                Column {
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    Label { text: qsTr("Password"); color: Theme.highlightColor }
                    Label {
                        x: Theme.paddingLarge
                        text: cipher.loginPassword ? cipher.loginPassword : " "
                        wrapMode: "WordWrap"
                    }
                }
            }

            BackgroundItem {
                visible: cipher.type === 1
                width: column.width
                height: contentItem.childrenRect.height
                Column {
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    Label { text: qsTr("Password revision date"); color: Theme.highlightColor }
                    Label {
                        x: Theme.paddingLarge
                        text: cipher.loginPasswordRevisionDate ? cipher.loginPasswordRevisionDate : qsTr("None")
                        wrapMode: "WordWrap"
                    }
                }
            }

            BackgroundItem {
                visible: cipher.type === 1
                width: column.width
                height: contentItem.childrenRect.height
                Column {
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    Label { text: qsTr("Uri"); color: Theme.highlightColor }
                    Label {
                        x: Theme.paddingLarge
                        text: cipher.loginUri ? cipher.loginUri : " "
                        wrapMode: "WordWrap"
                    }
                }
            }

            BackgroundItem {
                id: fieldsButton

                width: parent.width
                height: Theme.itemSizeMedium

                onClicked: pageStack.animatorPush(Qt.resolvedUrl("CipherFields.qml"))

                Rectangle {
                    anchors.fill: parent
                    gradient: Gradient {
                        GradientStop {
                            position: 0.0
                            color: Theme.rgba(fieldsButton.palette.highlightBackgroundColor, 0.1)
                        }
                        GradientStop {
                            position: 1.0
                            color: "transparent"
                        }
                    }
                }

                Label {
                    text: qsTr("Custom fields") + " (" + cipherFieldsListModel.count + ")"
                    color: fieldsButton.highlighted ? Theme.secondaryHighlightColor : Theme.highlightColor
                    anchors {
                        left: parent.left
                        leftMargin: Theme.horizontalPageMargin
                        right: parent.right
                        rightMargin: fieldsButtonIconRightImg.width + Theme.paddingSmall + Theme.horizontalPageMargin
                        verticalCenter: parent.verticalCenter
                    }
                    font.pixelSize: Theme.fontSizeLarge
                    truncationMode: TruncationMode.Fade
                }

                Icon {
                    id: fieldsButtonIconRightImg

                    anchors {
                        right: parent.right
                        rightMargin: Theme.horizontalPageMargin
                        verticalCenter: parent.verticalCenter
                    }
                    source: "image://theme/icon-m-right"
                    highlighted: fieldsButton.down
                }
            }


        }

    }

}
