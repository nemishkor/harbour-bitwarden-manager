#ifndef SYNCSERVICE_H
#define SYNCSERVICE_H

#include <QObject>
#include <QDateTime>
#include <QDebug>
#include <QList>
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSettings>

#include "src/factories/cipherfactory.h"
#include "src/factories/folderfactory.h"
#include "src/services/folderservice.h"
#include "src/services/stateservice.h"
#include "api.h"
#include "cryptoservice.h"
#include "cipher.h"
#include "src/models/taskslistmodel.h"
#include "src/services/cipherservice.h"
#include "cipherstring.h"
#include "folder.h"
#include "tokenservice.h"
#include "user.h"
#include "apijsondumper.h"

class SyncService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isSyncing READ getIsSyncing NOTIFY isSyncingChanged)
    Q_PROPERTY(bool synchronized READ isSynchronized NOTIFY synchronizedChanged);
    Q_PROPERTY(QDateTime lastSync READ getLastSync NOTIFY lastSyncChanged);
public:
    SyncService(Api *api, User *user, TokenService *tokenService,
                CryptoService *cryptoService, StateService *stateService,
                FolderService *foldersService,
                CipherService *cipherService, QSettings *settings,
                ApiJsonDumper *apiJsonDumper,
                TasksListModel *tasksListModel);
    Q_INVOKABLE void syncAll();
    Q_INVOKABLE void abort();
    bool getIsSyncing() const;
    QString getMessage() const;
    QString getMessageType() const;
    bool isSynchronized() const;
    QDateTime getLastSync() const;
    void clear();

private:
    Api *api;
    User *user;
    TokenService *tokenService;
    CryptoService *cryptoService;
    StateService *stateService;
    FolderService *foldersService;
    CipherService *cipherService;
    QSettings *settings;
    ApiJsonDumper *apiJsonDumper;
    CipherFactory *cipherFactory;
    FolderFactory *folderFactory;
    TasksListModel *tasksListModel;
    TaskListItem *syncingTask;

    // For GUI
    QDateTime lastSync;
    void setLastSync(const QDateTime &value);

    QNetworkReply *syncReply;
    void syncReplyFinished();

    void syncProfile(QJsonObject profile);
    void syncFolders(QString userId, QJsonArray folders);
    void syncCollections();
    void syncCiphers(QString userId, QJsonArray ciphers);
    void syncSends();
    void syncSettings();
    void syncPolicies();

signals:
    void isSyncingChanged();
    void messageChanged();
    void messageTypeChanged();
    void synchronizedChanged();
    void lastSyncChanged();

private slots:
    void syncingTaskWasUpdated();
    void refreshTokenSuccess();
    void refreshTokenFail(QString reason);

};

#endif // SYNCSERVICE_H
