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

#include "folderservice.h"
#include "stateservice.h"
#include "cipherservice.h"
#include "cryptoservice.h"
#include "tokenservice.h"
#include "src/api.h"
#include "src/apijsondumper.h"
#include "src/cipherstring.h"
#include "src/factories/cipherfactory.h"
#include "src/factories/folderfactory.h"
#include "src/models/cipher.h"
#include "src/models/taskslistmodel.h"
#include "src/tasks/synctask.h"
#include "src/user.h"

class SyncService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isSyncing READ getIsSyncing NOTIFY isSyncingChanged)
    Q_PROPERTY(bool synchronized READ isSynchronized NOTIFY synchronizedChanged);
    Q_PROPERTY(QDateTime lastSync READ getLastSync NOTIFY lastSyncChanged);
public:
    SyncService(CipherFactory *cipherFactory,
                FolderFactory *folderFactory,
                Api *api, User *user, TokenService *tokenService,
                CryptoService *cryptoService, StateService *stateService,
                FolderService *foldersService,
                CipherService *cipherService, QSettings *settings,
                ApiJsonDumper *apiJsonDumper,
                TasksListModel *tasksListModel, QObject* parent = nullptr);
    Q_INVOKABLE void syncAll();
    bool getIsSyncing() const;
    bool isSynchronized() const;
    QDateTime getLastSync() const;
    void clear();

private:
    CipherFactory *cipherFactory;
    FolderFactory *folderFactory;
    Api *api;
    User *user;
    TokenService *tokenService;
    CryptoService *cryptoService;
    StateService *stateService;
    FolderService *foldersService;
    CipherService *cipherService;
    QSettings *settings;
    ApiJsonDumper *apiJsonDumper;
    TasksListModel *tasksListModel;
    TaskListItem *syncingTask = nullptr;

    // For GUI
    QDateTime lastSync;
    void setLastSync(const QDateTime &value);

signals:
    void isSyncingChanged();
    void synchronizedChanged();
    void lastSyncChanged();

private slots:
    void syncingTaskWasFinished(TaskListItem*);

};

#endif // SYNCSERVICE_H
