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
    apiJsonDumper = new ApiJsonDumper();
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
    foldersModel->clear();
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
        foldersModel->add(folder);
    }
}

void SyncService::syncCollections()
{
    // TODO sync collections
}

void SyncService::syncCiphers(QString userId, QJsonArray ciphers)
{
    QJsonArray::const_iterator i, cipherChildArrIt;
    QJsonObject c, l, card, identity, cipherChildArrItem;
    QJsonArray fields, passwordHistory;

    for (i = ciphers.constBegin(); i != ciphers.constEnd(); i++){
        qDebug() << "add cipher";
        c = (*i).toObject();
        apiJsonDumper->dumpCipherFields(&c);
        Cipher cipher(CipherString(c["name"].toString()));
        cipher.setId(c["id"].toString());
        if(c["organizationId"].isString()) {
            cipher.setOrganizationId(c["organizationId"].toString());
        }
        if(c["folderId"].isString()) {
            cipher.setFolderId(c["folderId"].toString());
        }
        cipher.setUserId(userId);
        cipher.setEdit(c["edit"].toBool());
        cipher.setViewPassword(c["viewPassword"].toBool());
        cipher.setOrganizationUseTotp(c["organizationUseTotp"].toBool());
        cipher.setFavorite(c["favorite"].toBool());
        cipher.setRevisionDate(c["revisionDate"].toString());
        cipher.setType(static_cast<Cipher::CipherType>(c["type"].toInt()));
        cipher.setSizeName(c["sizeName"].toString());
        cipher.setNotes(c["notes"].toString());
        if(c["deletedDate"].isString()) {
            cipher.setDeletedDate(c["deletedDate"].toString());
        }

        if(c.contains("login") && c["login"].isObject()){
            l = c["login"].toObject();
            apiJsonDumper->dumpCipherLoginFields(&l);
            cipher.getLogin()->fillPassword(l["password"].toString());
            if(l["passwordRevisionDate"].isString()){
                cipher.getLogin()->setPasswordRevisionDate(l["passwordRevisionDate"].toString());
            }
            if(l["uri"].isString()) {
                cipher.getLogin()->fillUri(l["uri"].toString());
            }
            if(l["totp"].isString()) {
                cipher.getLogin()->fillTotp(l["totp"].toString());
            }
            cipher.getLogin()->fillUsername(l["username"].toString());
        }

        if(c.contains("card") && c["card"].isObject()){
            card = c["card"].toObject();
            apiJsonDumper->dumpCipherCardFields(&card);
            if(card["brand"].isString()){
                cipher.getCard()->fillBrand(card["brand"].toString());
            }
            if(card["cardholderName"].isString()){
                cipher.getCard()->fillCardholderName(card["cardholderName"].toString());
            }
            if(card["code"].isString()){
                cipher.getCard()->fillCode(card["code"].toString());
            }
            if(card["expMonth"].isString()){
                cipher.getCard()->fillExpMonth(card["expMonth"].toString());
            }
            if(card["expYear"].isString()){
                cipher.getCard()->fillExpYear(card["expYear"].toString());
            }
            if(card["number"].isString()){
                cipher.getCard()->fillNumber(card["number"].toString());
            }
        }

        if(c.contains("identity") && c["identity"].isObject()){
            qDebug() << "Sync identity cipher";
            identity = c["identity"].toObject();
            apiJsonDumper->dumpCipherIdentityFields(&identity);

            if(identity["address1"].isString()){
                cipher.getIdentity()->fillAddress1(identity["address1"].toString());
            }
            if(identity["address2"].isString()){
                cipher.getIdentity()->fillAddress2(identity["address1"].toString());
            }
            if(identity["address3"].isString()){
                cipher.getIdentity()->fillAddress3(identity["address3"].toString());
            }
            if(identity["city"].isString()){
                cipher.getIdentity()->fillCity(identity["city"].toString());
            }
            if(identity["company"].isString()){
                cipher.getIdentity()->fillCompany(identity["company"].toString());
            }
            if(identity["country"].isString()){
                cipher.getIdentity()->fillCountry(identity["country"].toString());
            }
            if(identity["email"].isString()){
                cipher.getIdentity()->fillEmail(identity["email"].toString());
            }
            if(identity["firstName"].isString()){
                cipher.getIdentity()->fillFirstName(identity["firstName"].toString());
            }
            if(identity["lastName"].isString()){
                cipher.getIdentity()->fillLastName(identity["lastName"].toString());
            }
            if(identity["licenseNumber"].isString()){
                cipher.getIdentity()->fillLicenseNumber(identity["licenseNumber"].toString());
            }
            if(identity["middleName"].isString()){
                cipher.getIdentity()->fillMiddleName(identity["middleName"].toString());
            }
            if(identity["passportNumber"].isString()){
                cipher.getIdentity()->fillPassportNumber(identity["passportNumber"].toString());
            }
            if(identity["phone"].isString()){
                cipher.getIdentity()->fillPhone(identity["phone"].toString());
            }
            if(identity["postalCode"].isString()){
                cipher.getIdentity()->fillPostalCode(identity["postalCode"].toString());
            }
            if(identity["ssn"].isString()){
                cipher.getIdentity()->fillSSN(identity["ssn"].toString());
            }
            if(identity["state"].isString()){
                cipher.getIdentity()->fillState(identity["state"].toString());
            }
            if(identity["title"].isString()){
                cipher.getIdentity()->fillTitle(identity["title"].toString());
            }
            if(identity["username"].isString()){
                cipher.getIdentity()->fillUsername(identity["username"].toString());
            }
            qDebug() << "Sync identity cipher finished";
        }

        fields = c["fields"].toArray();
        for(cipherChildArrIt = fields.constBegin(); cipherChildArrIt != fields.constEnd(); cipherChildArrIt++){
            cipherChildArrItem = (*cipherChildArrIt).toObject();
            cipher.addField(CipherField(
                CipherString(cipherChildArrItem["name"].toString()),
                static_cast<CipherField::FieldType>(cipherChildArrItem["type"].toInt()),
                CipherString(cipherChildArrItem["value"].toString())
            ));
        }

        if(c["passwordHistory"].isArray()){
            passwordHistory = c["passwordHistory"].toArray();
            for(cipherChildArrIt = passwordHistory.constBegin(); cipherChildArrIt != passwordHistory.constEnd(); cipherChildArrIt++){
                cipherChildArrItem = (*cipherChildArrIt).toObject();
                apiJsonDumper->dumpCipherPasswordHistoryFields(&cipherChildArrItem);
                cipher.addPasswordHistoryItem(CipherPasswordHistoryItem(
                    cipherChildArrItem["lastUsedDate"].toString(),
                    CipherString(cipherChildArrItem["password"].toString())
                ));
            }
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
