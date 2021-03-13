#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <QSettings>
#include <sailfishapp.h>
#include "api.h"
#include "auth.h"
#include "appidservice.h"
#include "foldersmodel.h"
#include "ciphersmodel.h"
#include "cryptoservice.h"
#include "syncservice.h"
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
    QSettings settings("harbour-bitwarden-manager");

    TokenService tokenService(&settings);
    context->setContextProperty("tokenService", &tokenService);

    Api api(QString("https://api.bitwarden.com"), QString("https://identity.bitwarden.com"));
    context->setContextProperty("api", &api);

    CryptoService crypto(&settings);
    AppIdService appIdService(&settings);

    User user(&settings);
    context->setContextProperty("user", &user);

    Auth auth(&appIdService, &tokenService, &api, &crypto, &user);
    context->setContextProperty("auth", &auth);

    FoldersModel foldersModel;
    context->setContextProperty("foldersModel", &foldersModel);

    CiphersModel ciphersModel;
    context->setContextProperty("ciphersModel", &ciphersModel);

    SyncService syncService(&api, &user, &tokenService, &crypto, &foldersModel, &ciphersModel, &settings);
    context->setContextProperty("syncService", &syncService);

    // Start the application.
    view->setSource(SailfishApp::pathTo("qml/harbour-bitwarden-manager.qml"));
    view->show();
    return app->exec();
}
