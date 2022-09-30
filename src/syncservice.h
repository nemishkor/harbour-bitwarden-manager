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

#include <src/factories/cipherfactory.h>

#include <src/services/folderservice.h>
#include <src/services/stateservice.h>

#include "api.h"
#include "cryptoservice.h"
#include "cipher.h"
#include "cipherservice.h"
#include "cipherstring.h"
#include "folder.h"
#include "tokenservice.h"
#include "user.h"
#include "apijsondumper.h"

class SyncService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isSyncing READ getIsSyncing NOTIFY isSyncingChanged)
    Q_PROPERTY(QString message READ getMessage NOTIFY messageChanged);
    Q_PROPERTY(QString messageType READ getMessageType NOTIFY messageTypeChanged);
    Q_PROPERTY(bool synchronized READ isSynchronized NOTIFY synchronizedChanged);
    Q_PROPERTY(QDateTime lastSync READ getLastSync NOTIFY lastSyncChanged);
public:
    SyncService(Api *api, User *user, TokenService *tokenService,
                CryptoService *cryptoService, StateService *stateService,
                FolderService *foldersService,
                CipherService *cipherService, QSettings *settings);
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

    // For GUI
    bool isSyncing = false;
    void setIsSyncing(bool value);
    QString message;
    QString messageType;
    void setMessage(QString message, QNetworkReply *reply);
    void setMessage(QString value, QString type = "info");
    QDateTime lastSync;
    void setLastSync(const QDateTime &value);

    bool refreshTokenRun = false;
    QNetworkReply *refreshTokenReply;
    QNetworkReply *syncReply;
    void refreshTokenReplyFinished();
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

};

#endif // SYNCSERVICE_H
