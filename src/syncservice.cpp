#include "syncservice.h"

SyncService::SyncService(Api *api, User *user, TokenService *tokenService, CryptoService *cryptoService, FoldersModel *foldersModel,
                         CiphersModel *ciphersModel,
                         QSettings *settings) :
    QObject(nullptr),
    api(api),
    user(user),
    tokenService(tokenService),
    cryptoService(cryptoService),
    foldersModel(foldersModel),
    ciphersModel(ciphersModel),
    settings(settings)
{
    syncReply = nullptr;
    refreshTokenReply = nullptr;
    if(settings->contains("last_sync_" + user->getUserId())){
        lastSync = settings->value("last_sync_" + user->getUserId()).toDateTime();
    }
}

void SyncService::syncAll()
{
    setIsSyncing(true);
    if(user->getUserId() == "" || !tokenService->exists()){
        setMessage("Can not sync. Not authorized!", "error");
        return;
    }
    if(tokenService->tokenNeedsRefresh()){
        if(refreshTokenRun){
            setMessage("Something went wrong. Token refreshed but needs refresh", "error");
            setIsSyncing(false);
            return;
        }
        setMessage("Token refreshing", "info");
        QString refreshToken = tokenService->getRefreshToken();
        if(refreshToken == ""){
            setMessage("Can not refresh access token. Refresh token is empty", "error");
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
    qDebug() << json.toJson();
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
    setMessage("Data downloaded. Syncing...", "info");
    QJsonDocument jsonDocument = QJsonDocument::fromJson(syncReply->readAll());
    qDebug() << jsonDocument.toJson();
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

    syncProfile(root["Profile"].toObject());
    if(!isSyncing){
        return;
    }

    syncFolders(userId, root["Folders"].toArray());
    if(!isSyncing){
        return;
    }

    syncCollections();
    if(!isSyncing){
        return;
    }

    syncCiphers(userId, root["Ciphers"].toArray());
    if(!isSyncing){
        return;
    }

    syncSends();
    if(!isSyncing){
        return;
    }

    syncSettings();
    if(!isSyncing){
        return;
    }

    syncPolicies();

    setLastSync(QDateTime::currentDateTime());

    setMessage("Done", "info");
    setIsSyncing(false);
}

void SyncService::syncProfile(QJsonObject profile)
{
    QString stamp = user->getStamp();
    if(stamp != "" && stamp != profile["SecurityStamp"].toString()){
        setMessage("Stamp has changed. Logout required", "error");
        setIsSyncing(false);
    }

    // TODO: ?
    cryptoService->setEncKey(profile["Key"].toString());
    // TODO: ?
    cryptoService->setPrivateKey(profile["PrivateKey"].toString());

    user->setStamp(profile["SecurityStamp"].toString());
    user->setEmail(profile["Email"].toString());
    user->setName(profile["Name"].toString());
    user->setPremium(profile["Premium"].toBool());

    // TODO sync user's organizations
}

void SyncService::syncFolders(QString userId, QJsonArray folders)
{
    foldersModel->clear();
    QJsonArray::const_iterator i;
    for (i = folders.constBegin(); i != folders.constEnd(); i++){
        Folder folder;
        folder.setId((*i).toObject()["Id"].toString());
        folder.setName((*i).toObject()["Name"].toString());
        folder.setUserId(userId);
        folder.setRevisionDate((*i).toObject()["RevisionDate"].toString());
        foldersModel->add(folder);
    }
}

void SyncService::syncCollections()
{
    // TODO sync collections
}

void SyncService::syncCiphers(QString userId, QJsonArray ciphers)
{
    ciphersModel->clear();
    QJsonArray::const_iterator i;
    for (i = ciphers.constBegin(); i != ciphers.constEnd(); i++){
        Cipher cipher;
        cipher.setId((*i).toObject()["Id"].toString());
        cipher.setOrganizationId((*i).toObject()["OrganizationId"].toString());
        cipher.setFolderId((*i).toObject()["FolderId"].toString());
        cipher.setUserId(userId);
        cipher.setEdit((*i).toObject()["Edit"].toBool());
        cipher.setViewPassword((*i).toObject()["ViewPassword"].toBool());
        cipher.setOrganizationUseTotp((*i).toObject()["OrganizationUseTotp"].toBool());
        cipher.setFavorite((*i).toObject()["Favorite"].toBool());
        cipher.setRevisionDate((*i).toObject()["RevisionDate"].toString());
//        cipher.setType(static_cast<Cipher::CipherType>((*i).toObject()["Type"].toInt()));
        cipher.setSizeName((*i).toObject()["SizeName"].toString());
        cipher.setName((*i).toObject()["Name"].toString());
        cipher.setNotes((*i).toObject()["Notes"].toString());
        cipher.setDeletedDate((*i).toObject()["DeletedDate"].toString());
        ciphersModel->add(cipher);
    }
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
