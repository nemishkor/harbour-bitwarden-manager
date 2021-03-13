#ifndef SYNCSERVICE_H
#define SYNCSERVICE_H

#include <QObject>
#include <QDateTime>
#include <QDebug>
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSettings>

#include "api.h"
#include "cryptoservice.h"
#include "ciphersmodel.h"
#include "folder.h"
#include "foldersmodel.h"
#include "tokenservice.h"
#include "user.h"

class SyncService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isSyncing READ getIsSyncing NOTIFY isSyncingChanged)
    Q_PROPERTY(QString message READ getMessage NOTIFY messageChanged);
    Q_PROPERTY(QString messageType READ getMessageType NOTIFY messageTypeChanged);
    Q_PROPERTY(bool synchronized READ isSynchronized NOTIFY synchronizedChanged);
    Q_PROPERTY(QDateTime lastSync READ getLastSync NOTIFY lastSyncChanged);
public:
    SyncService(Api *api, User *user, TokenService *tokenService, CryptoService *cryptoService, FoldersModel *foldersModel, CiphersModel *ciphersModel, QSettings *settings);
    Q_INVOKABLE void syncAll();
    Q_INVOKABLE void abort();
    bool getIsSyncing() const;
    QString getMessage() const;
    QString getMessageType() const;
    bool isSynchronized() const;
    QDateTime getLastSync() const;

private:
    Api *api;
    User *user;
    TokenService *tokenService;
    CryptoService *cryptoService;
    FoldersModel *foldersModel;
    CiphersModel *ciphersModel;
    QSettings *settings;

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
