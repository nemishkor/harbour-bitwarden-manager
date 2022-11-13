import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    function login(){
        auth.login(emailField.text, passwordField.text, apiKeyField.text)
    }

    RegExpValidator {
        id: urlValidator
        regExp: /(^https?:\/\/(www\.)?([a-zA-Z0-9]([a-zA-Z0-9\-]*)[a-zA-Z0-9]\.)+)([a-zA-Z0-9]{2,})$/
    }

    RegExpValidator {
        id: emailValidator
        regExp: /^[^\s]+@[^\s]+\.[^\s]+$/
    }

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors {
            fill: parent
        }

        clip: isPortrait

        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column

            width: page.width
            spacing: Theme.paddingLarge

            PageHeader {
                title: qsTr("Log In")
                description: "Bitwarden manager"
            }

            TextField {
                id: apiHostField
                readOnly: auth.loginStage > 0
                label: qsTr("Api host")
                text: environmentService.apiUrl
                width: parent.width
                validator: urlValidator
                EnterKey.enabled: acceptableInput
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: auth.preLogin(emailField.text)
                onTextChanged: environmentService.apiUrl = text
                inputMethodHints: Qt.ImhUrlCharactersOnly
            }

            TextField {
                id: identityHostField
                visible: auth.loginStage >= 0
                readOnly: auth.loginStage !== 0
                label: qsTr("Identity host")
                text: environmentService.identityUrl
                width: parent.width
                validator: urlValidator
                EnterKey.enabled: acceptableInput
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: auth.preLogin(emailField.text)
                onTextChanged: environmentService.identityUrl = text
                inputMethodHints: Qt.ImhUrlCharactersOnly
            }

            ButtonLayout {
                preferredWidth: Theme.buttonWidthExtraSmall
                Button {
                    visible: auth.loginStage === 0
                    text: qsTr("Set default URLs")
                    onClicked: {
                        environmentService.apiUrl = "https://api.bitwarden.com"
                        environmentService.identityUrl = "https://identity.bitwarden.com"
                    }
                }
            }

            TextField {
                id: emailField
                focus: true
                visible: auth.loginStage >= 0
                readOnly: auth.loginStage !== 0
                label: qsTr("Email")
                text: "nemish94@gmail.com"
                placeholderText: label
                width: parent.width
                validator: emailValidator
                EnterKey.enabled: acceptableInput
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: passwordField.forceActiveFocus()
                inputMethodHints: Qt.ImhEmailCharactersOnly
            }

            PasswordField {
                id: passwordField
                visible: auth.loginStage >= 0
                readOnly: auth.loginStage !== 0
                label: qsTr("Master password")
                width: parent.width
                validator: RegExpValidator { regExp: /^.+$/ }
                showEchoModeToggle: true
                EnterKey.enabled: acceptableInput
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: auth.apiKeyRequired ? apiKeyField.forceActiveFocus() : login()
            }

            PasswordField {
                id: apiKeyField
                visible: auth.loginStage >= 0 && auth.apiKeyRequired
                readOnly: auth.loginStage !== 0
                label: qsTr("API key")
                width: parent.width
                validator: auth.apiKeyRequired ? apiKeyFieldValidator : null
                showEchoModeToggle: true
                EnterKey.enabled: acceptableInput
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: login()
                RegExpValidator {
                    id: apiKeyFieldValidator
                    regExp: /^.+$/
                }
            }

            ButtonLayout {
                visible: auth.loginStage === 0
                Button {
                    preferredWidth: Theme.buttonWidthMedium
                    enabled: passwordField.acceptableInput && apiKeyField.acceptableInput
                    text: qsTr("Login")
                    onClicked: login()
                }
            }

        }

        VerticalScrollDecorator {}

    }

    onStatusChanged: {
        if (status == PageStatus.Active && auth.loginStage === 2) {
            console.log("User is authorized already")
            pageStack.animatorReplace(Qt.resolvedUrl("Home.qml"), {}, PageStackAction.Immediate)
        }
    }

    Connections {
        target: auth
        onLoginStageChanged: {
            if(auth.loginStage === 0){
                if(auth.apiKeyRequired){
                    apiKeyField.forceActiveFocus()
                } else {
                    passwordField.forceActiveFocus()
                }
            }
        }
    }

}
