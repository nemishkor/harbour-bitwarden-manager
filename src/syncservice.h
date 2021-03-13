#ifndef SYNCSERVICE_H
#define SYNCSERVICE_H

#include <QObject>
#include <QDebug>
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonDocument>

#include "api.h"
#include "cryptoservice.h"
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
public:
    SyncService(Api *api, User *user, TokenService *tokenService, CryptoService *cryptoService, FoldersModel *foldersModel);
    Q_INVOKABLE void syncAll();
    Q_INVOKABLE void abort();
    bool getIsSyncing() const;
    QString getMessage() const;
    QString getMessageType() const;

private:
    Api *api;
    User *user;
    TokenService *tokenService;
    CryptoService *cryptoService;
    FoldersModel *foldersModel;

    // For GUI
    bool isSyncing = false;
    void setIsSyncing(bool value);
    QString message;
    QString messageType;
    void setMessage(QString message, QNetworkReply *reply);
    void setMessage(QString value, QString type = "info");

    bool refreshTokenRun = false;
    QNetworkReply *refreshTokenReply;
    QNetworkReply *syncReply;
    void refreshTokenReplyFinished();
    void syncReplyFinished();

    void syncProfile(QJsonObject profile);
    void syncFolders(QString userId, QJsonArray folders);
    void syncCollections();
    void syncCiphers();
    void syncSends();
    void syncSettings();
    void syncPolicies();

signals:
    void isSyncingChanged();
    void messageChanged();
    void messageTypeChanged();

};

#endif // SYNCSERVICE_H
