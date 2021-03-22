import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"

ApplicationWindow
{
    initialPage: Component { Login { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations

    Connections {
        target: user
        onAuthenticatedChanged: {
            console.log('I do something cool when foo changes!')
//            while(pageStack.depth > 1){
//                pageStack.pop(null, PageStackAction.Immediate)
//            }

            pageStack.replaceAbove(null, Qt.resolvedUrl("pages/Login.qml"))
        }
    }

}
