#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <QGuiApplication>
#include <QSettings>
#include <QList>
#include <QQuickView>
#include <QQmlContext>
#include <QMutex>

#include <src/viewmodels/vault.h>

#include <sailfishapp.h>
#include "api.h"
#include "auth.h"
#include "appidservice.h"
#include "cipherview.h"
#include "src/services/cipherservice.h"
#include "cryptoservice.h"
#include "syncservice.h"
#include "services/environmentservice.h"
#include "tokenservice.h"
#include "vaultmanager.h"
#include "user.h"

static QMutex mutex; // global variable

void msgHandler(QtMsgType type, const QMessageLogContext & context, const QString & msg)
{
    mutex.lock();

    QDateTime dateTime(QDateTime::currentDateTime());

    QString timeStr(dateTime.toString("dd-MM-yyyy HH:mm:ss:zzz"));
    QString contextString(QString("(%1, %2)").arg(context.file).arg(context.line));

    QFile outFile("file.log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);

    QTextStream stream(&outFile);
    stream << timeStr << " " << contextString << ": " << msg << endl;

    mutex.unlock();
}

int main(int argc, char *argv[])
{
//    qInstallMessageHandler(msgHandler);

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

    EnvironmentService environmentService(&settings);
    context->setContextProperty("environmentService", &environmentService);

    Api api(&settings, &environmentService);
    context->setContextProperty("api", &api);

    CryptoService crypto(&settings);
    context->setContextProperty("crypto", &crypto);
    AppIdService appIdService(&settings);

    User user(&settings);
    context->setContextProperty("user", &user);

    CipherView cipher;
    context->setContextProperty("cipher", &cipher);

    CipherService cipherService(&crypto, &cipher);
    context->setContextProperty("cipherService", &cipherService);
    context->setContextProperty("ciphersListModel", cipherService.getCiphersListModel());
    context->setContextProperty("cipherFieldsListModel", cipherService.getCipherFieldsListModel());
    context->setContextProperty("cipherPasswordHistoryListModel", cipherService.getCipherPasswordHistoryListModel());

    StateService stateService;

    Vault vault(&stateService);
    context->setContextProperty("vault", &vault);

    FolderService foldersService(&stateService, &crypto);
    context->setContextProperty("foldersService", &foldersService);
    context->setContextProperty("foldersListModel", foldersService.getListModel());

    SyncService syncService(&api, &user, &tokenService, &crypto, &stateService, &foldersService,
                            &cipherService, &settings);
    context->setContextProperty("syncService", &syncService);

    Auth auth(&appIdService, &tokenService, &api, &crypto, &user, &syncService);
    context->setContextProperty("auth", &auth);

    VaultManager vaultManager(&crypto, &user, &api, &tokenService);
    context->setContextProperty("vaultManager", &vaultManager);

    QDateTime UTC(QDateTime::currentDateTimeUtc());
    QDateTime local(UTC.toLocalTime());
    qDebug() << "UTC time is:" << UTC;
    qDebug() << "Local time is:" << local;
    qDebug() << "No difference between times:" << UTC.secsTo(local);


    // Start the application.
    view->setSource(SailfishApp::pathTo("qml/harbour-bitwarden-manager.qml"));
    view->show();
    return app->exec();
}
