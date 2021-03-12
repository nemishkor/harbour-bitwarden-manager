import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    function login(){
        console.log("login")
        auth.login(passwordField.text)
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
                label: qsTr("Email")
                placeholderText: label
                width: parent.width
                validator: RegExpValidator { regExp: /^.+@.+$/ }
                EnterKey.enabled: acceptableInput
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: passwordField.focus = true
                text: auth.email
            }

            PasswordField {
                id: passwordField
                label: qsTr("Password")
                width: parent.width
                validator: RegExpValidator { regExp: /^.+$/ }
                EnterKey.enabled: acceptableInput
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: login()
            }

            ButtonLayout {
                Button {
                    id: loginBtn
                    enabled: emailField.acceptableInput && passwordField.acceptableInput
                    text: qsTr("Login")
                    onClicked: login()
                }
            }

        }

        VerticalScrollDecorator {}

    }

    DockedPanel {
        id: progressPanel

        open: auth.loginProcessing || auth.loginMessage !== ""
        width: page.isPortrait ? parent.width : Theme.itemSizeExtraLarge + Theme.paddingLarge
        height: page.isPortrait ? Theme.itemSizeExtraLarge + Theme.paddingLarge : parent.height

        dock: page.isPortrait ? Dock.Bottom : Dock.Right

        ProgressCircle {
            id: circle
            visible: auth.loginProcessing
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
                right: parent.theme
                rightMargin: Theme.horizontalPageMargin
            }
            padding: Theme.paddingMedium
            text: auth.loginMessage
            color: auth.loginMessageType === "error" ? Theme.errorColor : Theme.primaryColor
            wrapMode: "WrapAnywhere"
        }

    }

}
