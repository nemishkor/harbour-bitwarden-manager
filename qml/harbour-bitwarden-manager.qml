import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"
import "components"

ApplicationWindow
{
    initialPage: Component { Welcome { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations
    bottomMargin: statusBar.height

    Connections {
        target: user
        onAuthenticatedChanged: {
            pageStack.replaceAbove(null, Qt.resolvedUrl("pages/Welcome.qml"))
        }
    }

    Tasks {
        id: statusBar
    }

}
