import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"

ApplicationWindow
{
    initialPage: Component { Welcome { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations

    Connections {
        target: user
        onAuthenticatedChanged: {
            pageStack.replaceAbove(null, Qt.resolvedUrl("pages/Welcome.qml"))
        }
    }

}
