import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    function login(){
        auth.preLogin(emailField.text)
    }

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors {
            fill: parent
            rightMargin: page.isPortrait ? 0 : progressPanel.visibleSize
            bottomMargin: page.isPortrait ? progressPanel.visibleSize : 0
        }

        clip: isPortrait && progressPanel.expanded

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
            }

            TextField {
                id: emailField
                focus: true
                visible: auth.loginStage >= 0
                readOnly: auth.loginStage !== 0
                label: qsTr("Email")
                placeholderText: label
                width: parent.width
                validator: RegExpValidator { regExp: /^[^\s]+@[^\s]+\.[^\s]+$/ }
                EnterKey.enabled: acceptableInput
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: auth.preLogin(emailField.text)
            }

            ButtonLayout {
                visible: auth.loginStage === 0 || auth.loginStage === 2
                preferredWidth: auth.loginStage === 0 ? Theme.buttonWidthMedium : Theme.buttonWidthExtraSmall
                Button {
                    visible: auth.loginStage === 0
                    enabled: emailField.acceptableInput
                    text: qsTr("Next")
                    onClicked: auth.preLogin(emailField.text)
                }
                Button {
                    visible: auth.loginStage === 2
                    text: qsTr("Edit email")
                    onClicked: auth.reset()
                }
            }

            PasswordField {
                id: passwordField
                visible: auth.loginStage >= 2
                readOnly: auth.loginStage > 2
                label: qsTr("Password")
                width: parent.width
                validator: RegExpValidator { regExp: /^.+$/ }
                showEchoModeToggle: true
                EnterKey.enabled: acceptableInput
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: auth.login(passwordField.text)
            }

            ButtonLayout {
                visible: auth.loginStage === 2
                Button {
                    preferredWidth: Theme.buttonWidthMedium
                    enabled: passwordField.acceptableInput
                    text: qsTr("Login")
                    onClicked: auth.login(passwordField.text)
                }
            }

        }

        VerticalScrollDecorator {}

    }

    onStatusChanged: {
        if (status == PageStatus.Active && auth.loginStage === 4) { // if authorized already
            pageStack.animatorReplace(Qt.resolvedUrl("Home.qml"), {}, PageStackAction.Immediate)
        }
    }

    Connections {
        target: auth
        onLoginStageChanged: {
            console.log("login stage: " + auth.loginStage)
            if(auth.loginStage === 0){
                emailField.forceActiveFocus()
                passwordField.text = ""
            }
            if(auth.loginStage === 2){
                passwordField.forceActiveFocus()
            }
            if(auth.loginStage === 4){
                pageStack.animatorReplace(Qt.resolvedUrl("Home.qml"))
            }
        }
    }

    DockedPanel {
        id: progressPanel

        open: auth.loginProcessing || auth.loginMessage !== ""
        width: page.isPortrait ? parent.width : Theme.itemSizeExtraLarge + Theme.paddingLarge
        height: page.isPortrait ? Theme.itemSizeExtraLarge + Theme.paddingLarge : parent.height

        dock: page.isPortrait ? Dock.Bottom : Dock.Right

        ProgressCircle {
            id: circle
            visible: [-1, 1, 3].indexOf(auth.loginStage) !== -1
            anchors { left: parent.left; leftMargin: Theme.horizontalPageMargin; verticalCenter: parent.verticalCenter }

            NumberAnimation on value {
                from: 0
                to: 1
                duration: 1000
                running: progressPanel.expanded
                loops: Animation.Infinite
            }
        }

        Label {
            anchors {
                left: circle.visible ? circle.right : parent.left
                leftMargin: circle.visible ? Theme.paddingMedium : Theme.horizontalPageMargin
            }
            width: parent.width - 2 * Theme.horizontalPageMargin - (circle.visible ? circle.width : 0)
            padding: Theme.paddingMedium
            text: auth.loginMessage
            color: auth.loginMessageType === "error" ? Theme.errorColor : Theme.primaryColor
            wrapMode: "WrapAnywhere"
        }

    }

}
