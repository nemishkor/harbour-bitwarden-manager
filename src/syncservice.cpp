#include "syncservice.h"

SyncService::SyncService(Api *api, User *user, TokenService *tokenService,
                         CryptoService *cryptoService, StateService *stateService,
                         FolderService *foldersService,
                         CipherService *cipherService,
                         QSettings *settings,
                         ApiJsonDumper *apiJsonDumper,
                         TasksListModel *tasksListModel) :
    QObject(nullptr),
    api(api),
    user(user),
    tokenService(tokenService),
    cryptoService(cryptoService),
    stateService(stateService),
    foldersService(foldersService),
    cipherService(cipherService),
    settings(settings),
    apiJsonDumper(apiJsonDumper),
    tasksListModel(tasksListModel)
{
    syncReply = nullptr;
    refreshTokenReply = nullptr;
    syncingTask = nullptr;
    if(settings->contains("last_sync_" + user->getUserId())){
//        lastSync = settings->value("last_sync_" + user->getUserId()).toDateTime();
        settings->remove("last_sync_" + user->getUserId());
    }
    cipherFactory = new CipherFactory(apiJsonDumper);
    folderFactory = new FolderFactory(apiJsonDumper);
}

void SyncService::syncAll()
{
    if(syncingTask != nullptr){
        tasksListModel->remove(syncingTask);
    }

    TaskListItem task("Syncing");
    syncingTask = tasksListModel->add(task);
    connect(syncingTask, &TaskListItem::updated, this, &SyncService::syncingTaskWasUpdated);

    if(user->getUserId() == "" || !tokenService->exists()){
        syncingTask->fail("Can not sync. Not authorized!");
        return;
    }

    if(tokenService->tokenNeedsRefresh()){
        if(refreshTokenRun){
            syncingTask->fail("Oops. Something went wrong. Actual token is outdated");
            return;
        }
        syncingTask->setMessage("Refreshing token");
        QString refreshToken = tokenService->getRefreshToken();
        if(refreshToken == ""){
            syncingTask->fail("Oops. Something went wrong. Can not refresh access token. Refresh token is empty");
            return;
        }
        refreshTokenRun = true;
        refreshTokenReply = api->refreshAccessToken(tokenService->getClientIdFromToken(), refreshToken);
        connect(refreshTokenReply, &QNetworkReply::finished, this, &SyncService::refreshTokenReplyFinished);
        return;
    }

    syncingTask->setMessage("Downloading");
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
    syncingTask->success("Aborted");
}

bool SyncService::getIsSyncing() const
{
    return syncingTask != nullptr && syncingTask->getStatus() == Enums::TaskStatus::InProgress;
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

void SyncService::refreshTokenReplyFinished()
{
    if(refreshTokenReply->error() != QNetworkReply::NoError && refreshTokenReply->error() != QNetworkReply::ProtocolInvalidOperationError){
        syncingTask->fail("Token refreshing failed", refreshTokenReply);
        return;
    }

    QJsonDocument json = QJsonDocument::fromJson(refreshTokenReply->readAll());

    if(!json.isObject()){
        syncingTask->fail("Invalid refresh token API response #1");
        return;
    }

    QJsonObject root = json.object();

    if(refreshTokenReply->error() == QNetworkReply::ProtocolInvalidOperationError && root.contains("error")){
        syncingTask->fail("Token refreshing failed. " + root["error"].toString());
        return;
    }

    if(!root.contains("access_token")){
        syncingTask->fail("Token refreshing failed. API response does not contain access_token field");
        return;
    }

    if(!root.contains("refresh_token")){
        syncingTask->fail("Token refreshing failed. API response does not contain access_token field");
        return;
    }

    if(refreshTokenReply->error() == QNetworkReply::ProtocolInvalidOperationError){
        syncingTask->fail(refreshTokenReply);
        return;
    }

    tokenService->setTokens(root["access_token"].toString(), root["refresh_token"].toString());
    syncAll();
}

void SyncService::syncReplyFinished()
{
    clear();
    syncingTask->setMessage("Processing downloaded data");

    try {

        QJsonDocument jsonDocument = QJsonDocument::fromJson(syncReply->readAll());

        if(!jsonDocument.isObject()){
            syncingTask->fail("Invalid sync API response #1");
            return;
        }

        QString userId = user->getUserId();
        QJsonObject root = jsonDocument.object();
        apiJsonDumper->dumpSyncFields(&root);

        syncingTask->setMessage("profile");
        syncProfile(root["profile"].toObject());

        if(!getIsSyncing()){
            return;
        }

        syncingTask->setMessage("folders");
        syncFolders(userId, root["folders"].toArray());
        if(!getIsSyncing()){
            return;
        }

        syncingTask->setMessage("collections");
        syncCollections();
        if(!getIsSyncing()){
            return;
        }

        syncingTask->setMessage("ciphers");
        syncCiphers(userId, root["ciphers"].toArray());
        if(!getIsSyncing()){
            return;
        }

        syncingTask->setMessage("sends");
        syncSends();
        if(!getIsSyncing()){
            return;
        }

        syncingTask->setMessage("settings");
        syncSettings();
        if(!getIsSyncing()){
            return;
        }

        syncingTask->setMessage("policies");
        syncPolicies();

        setLastSync(QDateTime::currentDateTime());

        syncingTask->setMessage("");

    } catch (std::exception &e) {
        QString errorMessage;
        errorMessage.append("Failed: ").append(e.what()).append(" on \"").append(syncingTask->getMessage()).append("\"");
        syncingTask->fail(errorMessage);
        return;
    }

    syncingTask->success();
}

void SyncService::clear()
{
    qDebug() << "clear state";
    stateService->clear();
    qDebug() << "clear folders";
    foldersService->clear();
    qDebug() << "clear ciphers";
    cipherService->clear();
    qDebug() << "reset last sync";
    setLastSync(QDateTime());
}

void SyncService::syncProfile(QJsonObject profile)
{
    apiJsonDumper->dumpProfileFields(&profile);
    QString stamp = user->getStamp();
    if(stamp != "" && stamp != profile["securityStamp"].toString()){
        syncingTask->fail("Stamp has changed. Logout is required");
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
        Folder folder = folderFactory->create(apiFolder, userId);
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

void SyncService::syncingTaskWasUpdated()
{
    emit isSyncingChanged();
}
