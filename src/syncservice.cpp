#include "syncservice.h"

SyncService::SyncService(Api *api, User *user, TokenService *tokenService, CryptoService *cryptoService, FoldersModel *foldersModel,
                         CipherService *cipherService,
                         QSettings *settings) :
    QObject(nullptr),
    api(api),
    user(user),
    tokenService(tokenService),
    cryptoService(cryptoService),
    foldersModel(foldersModel),
    cipherService(cipherService),
    settings(settings)
{
    syncReply = nullptr;
    refreshTokenReply = nullptr;
    if(settings->contains("last_sync_" + user->getUserId())){
//        lastSync = settings->value("last_sync_" + user->getUserId()).toDateTime();
        settings->remove("last_sync_" + user->getUserId());
    }
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
    cipherService->clear();
    QJsonArray::const_iterator i, f;
    QJsonObject c, l, card, field;
    QJsonArray fields;
    for (i = ciphers.constBegin(); i != ciphers.constEnd(); i++){
        qDebug() << "add cipher";
        c = (*i).toObject();
        Cipher cipher(CipherString(c["Name"].toString()));
        cipher.setId(c["Id"].toString());
        if(c["OrganizationId"].isString()) {
            cipher.setOrganizationId(c["OrganizationId"].toString());
        }
        if(c["FolderId"].isString()) {
            cipher.setFolderId(c["FolderId"].toString());
        }
        cipher.setUserId(userId);
        cipher.setEdit(c["Edit"].toBool());
        cipher.setViewPassword(c["ViewPassword"].toBool());
        cipher.setOrganizationUseTotp(c["OrganizationUseTotp"].toBool());
        cipher.setFavorite(c["Favorite"].toBool());
        cipher.setRevisionDate(c["RevisionDate"].toString());
        cipher.setType(static_cast<Cipher::CipherType>(c["Type"].toInt()));
        cipher.setSizeName(c["SizeName"].toString());
        cipher.setNotes(c["Notes"].toString());
        cipher.setDeletedDate(c["DeletedDate"].toString());

        if(c.contains("Login") && c["Login"].isObject()){
            l = c["Login"].toObject();
            cipher.getLogin()->fillPassword(l["Password"].toString());
            if(l["PasswordRevisionDate"].isString()){
                cipher.getLogin()->setPasswordRevisionDate(l["PasswordRevisionDate"].toString());
            }
            if(l["Uri"].isString()) {
                cipher.getLogin()->fillUri(l["Uri"].toString());
            }
            if(l["Totp"].isString()) {
                cipher.getLogin()->fillTotp(l["Totp"].toString());
            }
            cipher.getLogin()->fillUsername(l["Username"].toString());
        }

        if(c.contains("Card") && c["Card"].isObject()){
            card = c["Card"].toObject();
            if(card["Brand"].isString()){
                cipher.getCard()->fillBrand(card["Brand"].toString());
            }
            if(card["CardholderName"].isString()){
                cipher.getCard()->fillCardholderName(card["CardholderName"].toString());
            }
            if(card["CardholderName"].isString()){
                cipher.getCard()->fillCode(card["CardholderName"].toString());
            }
            if(card["ExpMonth"].isString()){
                cipher.getCard()->fillExpMonth(card["ExpMonth"].toString());
            }
            if(card["ExpYear"].isString()){
                cipher.getCard()->fillExpYear(card["ExpYear"].toString());
            }
            if(card["Number"].isString()){
                cipher.getCard()->fillNumber(card["Number"].toString());
            }
        }

        fields = c["Fields"].toArray();
        for(f = fields.constBegin(); f != fields.constEnd(); f++){
            field = (*f).toObject();
            cipher.addField(CipherField(
                CipherString(field["Name"].toString()),
                static_cast<CipherField::FieldType>(field["Type"].toInt()),
                CipherString(field["Value"].toString())
            ));
        }

        cipherService->add(cipher);
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
