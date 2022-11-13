#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <QGuiApplication>
#include <QSettings>
#include <QList>
#include <QQuickView>
#include <QQmlContext>
#include <QMutex>
#include <sailfishapp.h>

#include "api.h"
#include "app.h"
#include "auth.h"
#include "src/models/taskslistmodel.h"
#include "src/viewmodels/vault.h"
#include "src/viewmodels/cipherview.h"
#include "src/services/appidservice.h"
#include "src/services/cipherservice.h"
#include "src/services/cryptoservice.h"
#include "src/services/environmentservice.h"
#include "src/services/syncservice.h"
#include "src/services/tokenservice.h"
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

class Goo{
public:
    Goo(QString name): name(name){}
    QString name;
};

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
    QScopedPointer<QGuiApplication> guiApp(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());

    QQmlContext *context = view.data()->rootContext();

    App app(*view);

    context->setContextProperty("tasks", app.getTasksListModel());
    context->setContextProperty("environmentService", app.getEnvironmentService());
    context->setContextProperty("api", app.getApi());
    context->setContextProperty("tokenService", app.getTokenService());
    context->setContextProperty("crypto", app.getCryptoService());
    context->setContextProperty("user", app.getUser());
    context->setContextProperty("cipher", app.getCipher());
    context->setContextProperty("cipherService", app.getCipherService());
    context->setContextProperty("ciphersListModel", app.getCipherService()->getCiphersListModel());
    context->setContextProperty("cipherFieldsListModel", app.getCipherService()->getCipherFieldsListModel());
    context->setContextProperty("cipherPasswordHistoryListModel", app.getCipherService()->getCipherPasswordHistoryListModel());
    context->setContextProperty("vault", app.getVault());
    context->setContextProperty("foldersService", app.getFoldersService());
    context->setContextProperty("foldersListModel", app.getFoldersService()->getListModel());
    context->setContextProperty("syncService", app.getSyncService());
    context->setContextProperty("auth", app.getAuth());
    context->setContextProperty("vaultManager", app.getVaultManager());
    context->setContextProperty("entitiesService", app.getEntitiesService());

    // Start the application.
    view->setSource(SailfishApp::pathTo("qml/harbour-bitwarden-manager.qml"));
    view->show();
    return guiApp->exec();
}
