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
            CipherLabel { cipherType: 1; label: qsTr("Username"); value: cipher.loginUsername }
            CipherLabel { cipherType: 1; label: qsTr("Password"); value: cipher.loginPassword ? cipher.loginPassword : " " }
            CipherLabel { cipherType: 1; label: qsTr("Password revision date"); value: cipher.loginPasswordRevisionDate ? cipher.loginPasswordRevisionDate : qsTr("None") }
            CipherLabel { cipherType: 1; label: qsTr("TOTP"); value: cipher.loginTotp ? cipher.loginTotp : " " }
            CipherLabel { cipherType: 1; label: qsTr("Uri"); value: cipher.loginUri ? cipher.loginUri : " " }

            SectionHeader { text: qsTr("Card"); visible: cipher.type === 3 }
            CipherLabel { cipherType: 3; label: qsTr("Brand"); value: cipher.cardBrand }
            CipherLabel { cipherType: 3; label: qsTr("Cardholder"); value: cipher.cardCardholder }
            CipherLabel { cipherType: 3; label: qsTr("Code"); value: cipher.cardCode }
            CipherLabel { cipherType: 3; label: qsTr("Exp month"); value: cipher.cardExpMonth }
            CipherLabel { cipherType: 3; label: qsTr("Exp year"); value: cipher.cardExpYear }
            CipherLabel { cipherType: 3; label: qsTr("Number"); value: cipher.cardNumber }

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

            BackgroundItemButton {
                page: "../pages/CipherFields.qml"
                label: qsTr("Custom fields") + " (" + cipherFieldsListModel.count + ")"
            }
            BackgroundItemButton {
                page: "../pages/CipherPasswordHistory.qml"
                label: qsTr("Password history") + " (" + cipherPasswordHistoryListModel.count + ")"
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
