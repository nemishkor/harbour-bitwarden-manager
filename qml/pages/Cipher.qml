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
        anchors.fill: parent

        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height + Theme.paddingLarge

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
                visible: cipher.deletedDate !== ""
                x: Theme.horizontalPageMargin
                height: Theme.iconSizeMedium
                width: parent.width - 2 * Theme.horizontalPageMargin

                Icon {
                    source: "image://theme/icon-m-delete"
                }
                Label {
                    height: Theme.iconSizeMedium
                    x: Theme.iconSizeMedium + Theme.paddingSmall
                    color: Theme.primaryColor
                    verticalAlignment: Text.AlignVCenter
                    text: "Is deleted: " + cipher.deletedDate
                }
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
                    Label { text: qsTr("Username") }
                    Label {
                        text: cipher.loginUsername
                        font.pixelSize: Theme.fontSizeExtraSmall
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
                    Label { text: qsTr("Password") }
                    Label {
                        text: cipher.loginPassword ? cipher.loginPassword : " "
                        font.pixelSize: Theme.fontSizeExtraSmall
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
                    Label { text: qsTr("Password revision date") }
                    Label {
                        text: cipher.loginPasswordRevisionDate ? cipher.loginPasswordRevisionDate : qsTr("None")
                        font.pixelSize: Theme.fontSizeExtraSmall
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
                    Label { text: qsTr("TOTP") }
                    Label {
                        text: cipher.loginTotp ? cipher.loginTotp : " "
                        font.pixelSize: Theme.fontSizeExtraSmall
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
                    Label { text: qsTr("Uri") }
                    Label {
                        text: cipher.loginUri ? cipher.loginUri : " "
                        font.pixelSize: Theme.fontSizeExtraSmall
                        wrapMode: "WordWrap"
                    }
                }
            }

            SectionHeader { text: qsTr("Card"); visible: cipher.type === 3 }

            BackgroundItem {
                visible: cipher.type === 3
                width: column.width
                height: contentItem.childrenRect.height
                Column {
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    Label { text: qsTr("Brand") }
                    Label {
                        text: cipher.cardBrand
                        font.pixelSize: Theme.fontSizeExtraSmall
                        wrapMode: "WordWrap"
                    }
                }
            }

            BackgroundItem {
                visible: cipher.type === 3
                width: column.width
                height: contentItem.childrenRect.height
                Column {
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    Label { text: qsTr("Cardholder") }
                    Label {
                        text: cipher.cardCardholder
                        font.pixelSize: Theme.fontSizeExtraSmall
                        wrapMode: "WordWrap"
                    }
                }
            }

            BackgroundItem {
                visible: cipher.type === 3
                width: column.width
                height: contentItem.childrenRect.height
                Column {
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    Label { text: qsTr("Code") }
                    Label {
                        text: cipher.cardCode
                        font.pixelSize: Theme.fontSizeExtraSmall
                        wrapMode: "WordWrap"
                    }
                }
            }

            BackgroundItem {
                visible: cipher.type === 3
                width: column.width
                height: contentItem.childrenRect.height
                Column {
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    Label { text: qsTr("Exp month") }
                    Label {
                        text: cipher.cardExpMonth
                        font.pixelSize: Theme.fontSizeExtraSmall
                        wrapMode: "WordWrap"
                    }
                }
            }

            BackgroundItem {
                visible: cipher.type === 3
                width: column.width
                height: contentItem.childrenRect.height
                Column {
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    Label { text: qsTr("Exp year") }
                    Label {
                        text: cipher.cardExpYear
                        font.pixelSize: Theme.fontSizeExtraSmall
                        wrapMode: "WordWrap"
                    }
                }
            }

            BackgroundItem {
                visible: cipher.type === 3
                width: column.width
                height: contentItem.childrenRect.height
                Column {
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    Label { text: qsTr("Number") }
                    Label {
                        text: cipher.cardNumber
                        font.pixelSize: Theme.fontSizeExtraSmall
                        wrapMode: "WordWrap"
                    }
                }
            }

            SectionHeader { text: qsTr("Identity"); visible: cipher.type === 4 }
            CipherLabel { cipherType: 4; label: qsTr("Title"); value: cipher.identityTitle }
            CipherLabel { cipherType: 4; label: qsTr("First name"); value: cipher.identityFirstName }
            CipherLabel { cipherType: 4; label: qsTr("Middle name"); value: cipher.identityMiddleName }
            CipherLabel { cipherType: 4; label: qsTr("Last name"); value: cipher.identityLastName }
            CipherLabel { cipherType: 4; label: qsTr("Username"); value: cipher.identityUsername }
            CipherLabel { cipherType: 4; label: qsTr("Company"); value: cipher.identityCompany }
            CipherLabel { cipherType: 4; label: qsTr("SSN"); value: cipher.identitySSN }
            CipherLabel { cipherType: 4; label: qsTr("Passport number"); value: cipher.identityPassportNumber }
            CipherLabel { cipherType: 4; label: qsTr("License number"); value: cipher.identityLicenseNumber }
            CipherLabel { cipherType: 4; label: qsTr("Email"); value: cipher.identityEmail }
            CipherLabel { cipherType: 4; label: qsTr("Phone"); value: cipher.identityPhone }
            CipherLabel { cipherType: 4; label: qsTr("Address 1"); value: cipher.identityAddress1 }
            CipherLabel { cipherType: 4; label: qsTr("Address 2"); value: cipher.identityAddress2 }
            CipherLabel { cipherType: 4; label: qsTr("Address 3"); value: cipher.identityAddress3 }
            CipherLabel { cipherType: 4; label: qsTr("City"); value: cipher.identityCity }
            CipherLabel { cipherType: 4; label: qsTr("State"); value: cipher.identityState }
            CipherLabel { cipherType: 4; label: qsTr("Postal code"); value: cipher.identityPostalCode }
            CipherLabel { cipherType: 4; label: qsTr("Country"); value: cipher.identityCountry }

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

            BackgroundItem {
                visible: cipher.revisionDate !== ""
                width: column.width
                height: contentItem.childrenRect.height
                Column {
                    x: Theme.horizontalPageMargin
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    Label { text: qsTr("Revision date") }
                    Label {
                        text: cipher.revisionDate
                        font.pixelSize: Theme.fontSizeExtraSmall
                        wrapMode: "WordWrap"
                    }
                }
            }

        }

        VerticalScrollDecorator {}

    }

}
