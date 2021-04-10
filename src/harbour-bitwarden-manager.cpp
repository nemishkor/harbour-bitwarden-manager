#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <QGuiApplication>
#include <QSettings>
#include <QList>
#include <QQuickView>
#include <QQmlContext>

#include <sailfishapp.h>
#include "api.h"
#include "auth.h"
#include "appidservice.h"
#include "foldersmodel.h"
#include "cipherview.h"
#include "cipherservice.h"
#include "cryptoservice.h"
#include "syncservice.h"
#include "tokenservice.h"
#include "vaultmanager.h"
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

    Api api(&settings);
    context->setContextProperty("api", &api);

    CryptoService crypto(&settings);
    context->setContextProperty("crypto", &crypto);
    AppIdService appIdService(&settings);

    User user(&settings);
    context->setContextProperty("user", &user);

    Auth auth(&appIdService, &tokenService, &api, &crypto, &user);
    context->setContextProperty("auth", &auth);

    FoldersModel foldersModel;
    context->setContextProperty("foldersModel", &foldersModel);

    CipherView cipher;
    context->setContextProperty("cipher", &cipher);

    CipherService cipherService(&crypto, &cipher);
    context->setContextProperty("cipherService", &cipherService);
    context->setContextProperty("ciphersListModel", cipherService.getCiphersListModel());

    SyncService syncService(&api, &user, &tokenService, &crypto, &foldersModel, &cipherService, &settings);
    context->setContextProperty("syncService", &syncService);

    VaultManager vaultManager(&crypto, &user, &api, &tokenService);
    context->setContextProperty("vaultManager", &vaultManager);

    // Start the application.
    view->setSource(SailfishApp::pathTo("qml/harbour-bitwarden-manager.qml"));
    view->show();
    return app->exec();
}
