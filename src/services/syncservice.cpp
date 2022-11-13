#include "syncservice.h"

SyncService::SyncService(CipherFactory *cipherFactory,
                         FolderFactory *folderFactory,
                         Api *api,
                         User *user,
                         TokenService *tokenService,
                         CryptoService *cryptoService,
                         StateService *stateService,
                         FolderService *foldersService,
                         CipherService *cipherService,
                         QSettings *settings,
                         ApiJsonDumper *apiJsonDumper,
                         TasksListModel *tasksListModel,
                         QObject* parent) :
    QObject(parent),
    cipherFactory(cipherFactory),
    folderFactory(folderFactory),
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
    if(settings->contains("last_sync")){
//        lastSync = settings->value("last_sync").toDateTime();
        settings->remove("last_sync");
    }
}

void SyncService::syncAll()
{
    if(syncingTask != nullptr){
        qWarning() << "Syncing task is exist already. Unable to create another one";
        return;
    }
    syncingTask = tasksListModel->create("Syncing",
                                         new SyncTask(user, apiJsonDumper, cryptoService,
                                                      stateService, cipherFactory, folderFactory,
                                                      api, tokenService));
    clear();
    connect(syncingTask, &TaskListItem::finished, this, &SyncService::syncingTaskWasFinished);
    syncingTask->start();
    emit isSyncingChanged();
}

bool SyncService::getIsSyncing() const
{
    return syncingTask != nullptr;
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
    if(!lastSync.isValid() && !value.isValid()){
        return;
    }
    if(lastSync.isValid() && value.isValid() && lastSync == value){
        return;
    }
    lastSync = value;
    emit synchronizedChanged();
    emit lastSyncChanged();
    if(value.isValid()){
        settings->setValue("last_sync", lastSync);
    } else {
        settings->remove("last_sync");
    }
    settings->sync();
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

void SyncService::syncingTaskWasFinished(TaskListItem*)
{
    syncingTask = nullptr;
    emit isSyncingChanged();
    setLastSync(QDateTime::currentDateTime());
}
