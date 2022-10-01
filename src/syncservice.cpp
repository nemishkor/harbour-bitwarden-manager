#include "syncservice.h"

SyncService::SyncService(Api *api, User *user, TokenService *tokenService,
                         CryptoService *cryptoService, StateService *stateService,
                         FolderService *foldersService,
                         CipherService *cipherService,
                         QSettings *settings) :
    QObject(nullptr),
    api(api),
    user(user),
    tokenService(tokenService),
    cryptoService(cryptoService),
    stateService(stateService),
    foldersService(foldersService),
    cipherService(cipherService),
    settings(settings)
{
    syncReply = nullptr;
    refreshTokenReply = nullptr;
    if(settings->contains("last_sync_" + user->getUserId())){
//        lastSync = settings->value("last_sync_" + user->getUserId()).toDateTime();
        settings->remove("last_sync_" + user->getUserId());
    }
    apiJsonDumper = new ApiJsonDumper();
    cipherFactory = new CipherFactory(apiJsonDumper);
}

void SyncService::syncAll()
{
    setIsSyncing(true);
    if(user->getUserId() == "" || !tokenService->exists()){
        setMessage("Can not sync. Not authorized!", "error");
        setIsSyncing(false);
        return;
    }
    if(tokenService->tokenNeedsRefresh()){
        if(refreshTokenRun){
            setMessage("Oops. Something went wrong. Actual token is outdated", "error");
            setIsSyncing(false);
            return;
        }
        setMessage("Refreshing token", "info");
        QString refreshToken = tokenService->getRefreshToken();
        if(refreshToken == ""){
            setMessage("Oops. Something went wrong. Can not refresh access token. Refresh token is empty", "error");
            setIsSyncing(false);
            return;
        }
        refreshTokenRun = true;
        refreshTokenReply = api->refreshAccessToken(tokenService->getClientIdFromToken(), refreshToken);
        connect(refreshTokenReply, &QNetworkReply::finished, this, &SyncService::refreshTokenReplyFinished);
        return;
    }
    setMessage("Syncing", "info");
    syncReply = api->getSync(tokenService->getAccessToken());
    connect(syncReply, &QNetworkReply::finished, this, &SyncService::syncReplyFinished);
}

void SyncService::abort()
{
    if(refreshTokenReply && refreshTokenReply->isRunning()){
        refreshTokenReply->abort();
    }
    if(syncReply && syncReply->isRunning()){
        syncReply->abort();
    }
    setMessage("Aborted");
    setIsSyncing(false);
}

bool SyncService::getIsSyncing() const
{
    return isSyncing;
}

void SyncService::setIsSyncing(bool value)
{
    if(isSyncing != value){
        isSyncing = value;
        emit isSyncingChanged();
    }
}

void SyncService::setMessage(QString message, QNetworkReply *reply)
{
    setMessage((message == "" ? "" : message + ". ") + "[" + QString::number(reply->error()) + "]" + reply->errorString(), "error");
}

void SyncService::setMessage(QString value, QString type)
{
    qDebug().nospace() << "[" << type << "] Message: " << value;
    if(value != message){
        message = value;
        emit messageChanged();
    }
    if(messageType != type){
        messageType = type;
        emit messageTypeChanged();
    }
}

bool SyncService::isSynchronized() const
{
    return lastSync.isValid();
}

QDateTime SyncService::getLastSync() const
{
    return lastSync;
}

void SyncService::setLastSync(const QDateTime &value)
{
    if(lastSync != value){
        lastSync = value;
        emit synchronizedChanged();
        emit lastSyncChanged();
        settings->setValue("last_sync_" + user->getUserId(), lastSync);
        settings->sync();
    }
}

QString SyncService::getMessageType() const
{
    return messageType;
}

QString SyncService::getMessage() const
{
    return message;
}

void SyncService::refreshTokenReplyFinished()
{
    if(refreshTokenReply->error() != QNetworkReply::NoError && refreshTokenReply->error() != QNetworkReply::ProtocolInvalidOperationError){
        setMessage("Token refreshing failed", refreshTokenReply);
//        QList<QPair<QByteArray, QByteArray>>::const_iterator i;
//        for (i = refreshTokenReply->rawHeaderPairs().begin(); i != refreshTokenReply->rawHeaderPairs().end(); ++i)
//            qDebug() << "header:" << (*i).first << ":" << (*i).second;
//        qDebug() << "body: " << refreshTokenReply->readAll();
        return;
    }
    QJsonDocument json = QJsonDocument::fromJson(refreshTokenReply->readAll());
    if(!json.isObject()){
        setMessage("Invalid refresh token API response #1", "error");
        setIsSyncing(false);
        return;
    }
    QJsonObject root = json.object();
    if(refreshTokenReply->error() == QNetworkReply::ProtocolInvalidOperationError && root.contains("error")){
        setMessage("Token refreshing failed. " + root["error"].toString(), "error");
        setIsSyncing(false);
        return;
    }
    if(!root.contains("access_token")){
        setMessage("Token refreshing failed. API response does not contain access_token field", "error");
        setIsSyncing(false);
        return;
    }
    if(!root.contains("refresh_token")){
        setMessage("Token refreshing failed. API response does not contain access_token field", "error");
        setIsSyncing(false);
        return;
    }


    if(refreshTokenReply->error() == QNetworkReply::ProtocolInvalidOperationError){
        setMessage("", refreshTokenReply);
        setIsSyncing(false);
        return;
    }

    tokenService->setTokens(root["access_token"].toString(), root["refresh_token"].toString());
    syncAll();
}

void SyncService::syncReplyFinished()
{
    clear();
    setMessage("Data is downloaded. Syncing...", "info");

    try {

        QJsonDocument jsonDocument = QJsonDocument::fromJson(syncReply->readAll());
        if(!jsonDocument.isObject()){
            setMessage("Invalid sync API response #1", "error");
            setIsSyncing(false);
            return;
        }

        QString userId = user->getUserId();
        QJsonObject root = jsonDocument.object();
        if(!isSyncing){
            return;
        }

        apiJsonDumper->dumpSyncFields(&root);

        setMessage("Syncing profile...", "info");
        syncProfile(root["profile"].toObject());
        if(!isSyncing){
            return;
        }

        setMessage("Syncing folders...", "info");
        syncFolders(userId, root["folders"].toArray());
        if(!isSyncing){
            return;
        }

        setMessage("Syncing collections...", "info");
        syncCollections();
        if(!isSyncing){
            return;
        }

        setMessage("Syncing ciphers...", "info");
        syncCiphers(userId, root["ciphers"].toArray());
        if(!isSyncing){
            return;
        }

        setMessage("Syncing sends...", "info");
        syncSends();
        if(!isSyncing){
            return;
        }

        setMessage("Syncing settings...", "info");
        syncSettings();
        if(!isSyncing){
            return;
        }

        setMessage("Syncing policies...", "info");
        syncPolicies();

        setLastSync(QDateTime::currentDateTime());

    } catch (std::exception &e) {
        QString errorMessage;
        errorMessage.append("Failed: ").append(e.what()).append(" on \"").append(message).append("\"");
        setMessage(errorMessage, "error");
        setIsSyncing(false);
        return;
    }

    setMessage("Done", "info");
    setIsSyncing(false);
}

void SyncService::clear()
{
    stateService->clear();
    foldersService->clear();
    cipherService->clear();
    setLastSync(QDateTime());
}

void SyncService::syncProfile(QJsonObject profile)
{
    apiJsonDumper->dumpProfileFields(&profile);
    QString stamp = user->getStamp();
    if(stamp != "" && stamp != profile["securityStamp"].toString()){
        setMessage("Stamp has changed. Logout required", "error");
        setIsSyncing(false);
    }

    cryptoService->setEncKey(profile["key"].toString());
    // TODO: ?
    cryptoService->setPrivateKey(profile["privateKey"].toString());

    user->setStamp(profile["securityStamp"].toString());
    user->setEmail(profile["email"].toString());
    user->setName(profile["name"].toString());
    user->setPremium(profile["premium"].toBool());

    // TODO sync user's organizations
}

void SyncService::syncFolders(QString userId, QJsonArray folders)
{
    QJsonObject apiFolder;
    QJsonArray::const_iterator i;
    for (i = folders.constBegin(); i != folders.constEnd(); i++){
        apiFolder = (*i).toObject();
        apiJsonDumper->dumpFolderFields(&apiFolder);
        Folder folder;
        folder.setId(apiFolder["id"].toString());
        folder.setName(apiFolder["name"].toString());
        folder.setUserId(userId);
        folder.setRevisionDate(apiFolder["revisionDate"].toString());
        stateService->add(folder);
    }
}

void SyncService::syncCollections()
{
    // TODO sync collections
}

void SyncService::syncCiphers(QString userId, QJsonArray ciphers)
{
    QJsonArray::const_iterator i;
    QJsonObject c;

    for (i = ciphers.constBegin(); i != ciphers.constEnd(); i++){
        qDebug() << "Add cipher";
        c = (*i).toObject();
        Cipher cipher = cipherFactory->create(c, userId);
        stateService->add(cipher);
    }

    qDebug() << "Ciphers were added";
}

void SyncService::syncSends()
{

}

void SyncService::syncSettings()
{

}

void SyncService::syncPolicies()
{

}
