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
    syncingTask = tasksListModel->create("Syncing", syncingTask);
    connect(syncingTask, &TaskListItem::updated, this, &SyncService::syncingTaskWasUpdated);

    if(user->getUserId() == "" || !tokenService->exists()){
        syncingTask->fail("Can not sync. Not authorized!");
        return;
    }

    connect(tokenService, &TokenService::refreshTokenSuccess, this, &SyncService::refreshTokenSuccess);
    connect(tokenService, &TokenService::refreshTokenFail, this, &SyncService::refreshTokenFail);
    tokenService->validateToken();
}

void SyncService::refreshTokenSuccess()
{
    disconnect(tokenService, &TokenService::refreshTokenSuccess, this, &SyncService::refreshTokenSuccess);
    disconnect(tokenService, &TokenService::refreshTokenFail, this, &SyncService::refreshTokenFail);
    syncingTask->setMessage("Downloading");
    syncReply = api->getSync(tokenService->getAccessToken());
    connect(syncReply, &QNetworkReply::finished, this, &SyncService::syncReplyFinished);
}

void SyncService::refreshTokenFail(QString reason)
{
    disconnect(tokenService, &TokenService::refreshTokenSuccess, this, &SyncService::refreshTokenSuccess);
    disconnect(tokenService, &TokenService::refreshTokenFail, this, &SyncService::refreshTokenFail);
    syncingTask->fail(reason);
}

void SyncService::abort()
{
    tokenService->abort();
    if(syncReply && syncReply->isRunning()){
        syncReply->abort();
    }
    syncingTask->success("Aborted");
}

bool SyncService::getIsSyncing() const
{
    return syncingTask != nullptr &&
            tasksListModel->contains(syncingTask) &&
            syncingTask->getStatus() == Enums::TaskStatus::InProgress;
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

