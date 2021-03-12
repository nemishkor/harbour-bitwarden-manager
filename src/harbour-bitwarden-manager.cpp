#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <QSettings>
#include <sailfishapp.h>
#include "api.h"
#include "cryptoservice.h"
#include "auth.h"
#include "appidservice.h"
#include "tokenservice.h"
#include "user.h"

int main(int argc, char *argv[])
{
    // SailfishApp::main() will display "qml/harbour-bitwarden-manager.qml", if you need more
    // control over initialization, you can use:
    //
    //   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
    //   - SailfishApp::createView() to get a new QQuickView * instance
    //   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
    //   - SailfishApp::pathToMainQml() to get a QUrl to the main QML file
    //
    // To display the view, call "show()" (will show fullscreen on device).

    // Set up QML engine.
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());

    QQmlContext *context = view.data()->rootContext();

    Api api(QString("https://api.bitwarden.com"), QString("https://identity.bitwarden.com"));
    context->setContextProperty("api", &api);
    QSettings settings("harbour-bitwarden-manager");
    CryptoService crypto;
    AppIdService appIdService(&settings);
    TokenService tokenService(&settings);
    User authorizedUser;
    Auth auth(&appIdService, &tokenService, &api, &crypto, &authorizedUser);
    context->setContextProperty("auth", &auth);

    // Start the application.
    view->setSource(SailfishApp::pathTo("qml/harbour-bitwarden-manager.qml"));
    view->show();
    return app->exec();
}
