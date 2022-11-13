import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"
import "components"

ApplicationWindow
{

    function isAuthorized(){
        return auth.loginStage === 2;
    }

    initialPage: Qt.resolvedUrl(isAuthorized() ? "pages/Home.qml" : "pages/Welcome.qml")
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations
    bottomMargin: statusBar.height

    Connections {
        target: auth
        onLoginStageChanged: {
            if(auth.loginStage === 2) {
                console.log("Open Home page")
                pageStack.replaceAbove(null, Qt.resolvedUrl("pages/Home.qml"))
            } else if(auth.loginStage === 0) {
                console.log("Open Welcome page")
                pageStack.replaceAbove(null, Qt.resolvedUrl("pages/Welcome.qml"))
            }
        }
    }

    Tasks {
        id: statusBar
    }

}
