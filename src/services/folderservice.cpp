#include "folderservice.h"

FolderService::FolderService(FolderFactory *folderFactory,
                             StateService *stateService,
                             CryptoService *cryptoService,
                             TokenService *tokenService,
                             TasksListModel *tasksListModel,
                             Api *api, QObject *parent):
    QObject(parent),
    folderFactory(folderFactory),
    stateService(stateService),
    cryptoService(cryptoService),
    tokenService(tokenService),
    api(api),
    tasksListModel(tasksListModel)
{
    listModel = new FoldersListModel(this);
    locale = QLocale::system();
    savingListItemFolder = nullptr;
    createFolderTask = nullptr;
    removeFoldersTask = nullptr;
}

void FolderService::addListItemToModel(Folder *folder)
{
    QDateTime revisionDate;
    QString isoRevisionDate;
    QString dateTimeFormat = locale.dateTimeFormat(QLocale::ShortFormat);
    FolderListItem listItem;
    listItem.setId(folder->getId());
    listItem.setName(cryptoService->decryptToUtf8(folder->getName()));
    // removing redundant part of the dateTime string
    // Bitwarden returns as example:
    // 2022-09-30T20:23:23.2123432Z
    // We need only "2022-09-30T20:23:23" part
    isoRevisionDate = folder->getRevisionDate().mid(0, 19);
    revisionDate = QDateTime::fromString(isoRevisionDate, Qt::ISODate);
    // Bitwarden returns time in UTC
    revisionDate.setTimeSpec(Qt::UTC);
    listItem.setRevisionDate(revisionDate.toLocalTime().toString(dateTimeFormat));
    listModel->add(listItem);
}

void FolderService::createFolderTaskWasUpdated()
{
    emit creatingChanged();
}

void FolderService::removeFolderTaskWasUpdated()
{
    emit removingChanged();
}

void FolderService::decryptAll()
{
    if(decrypted){
        qDebug() << "Folders are already decrypted. Using in-memory cache";
        return;
    }
    QList<Folder>::iterator i;
    QList<Folder> *folders = stateService->getFolders();
    for(i = folders->begin(); i != folders->end(); i++){
        addListItemToModel(&*i);
    }
    qDebug() << "Folders were decrypted";

    FolderListItem emptyFolder;
    emptyFolder.setId("");
    emptyFolder.setName(tr("None folder"));
    emptyFolder.setRevisionDate("");
    listModel->add(emptyFolder);

    decrypted = true;
}

void FolderService::create(QString name)
{
    if(createFolderTask != nullptr) {
        qWarning() << "create folder task is exist already. Unable to create another one";
        return;
    }
    createFolderTask = tasksListModel->create("Add folder");
    connect(createFolderTask, &TaskListItem::finished, this, &FolderService::createFolderTaskWasFinished);
    connect(createFolderTask, &TaskListItem::updated, this, &FolderService::createFolderTaskWasUpdated);
    createFolderTask->start();
    emit creatingChanged();
    FolderListItem folderListItem;
    folderListItem.setName(name);
    listModel->add(folderListItem);
    setSavingListItemFolder(&folderListItem);

    createFolderTask->setMessage("Validating token");
    connect(tokenService, &TokenService::refreshTokenSuccess, this, &FolderService::postFolderRefreshTokenSuccess);
    connect(tokenService, &TokenService::refreshTokenFail, this, &FolderService::postFolderRefreshTokenFailed);
    tokenService->validateToken();
}

void FolderService::removeSelected()
{
    if(isRemoving()){
        qCritical() << "Unable to run 'remove' task. It's in progress already";
        return;
    }


    // TODO:
//    setRemoveFoldersTask(tasksListModel->create("Remove folder(s)", removeFoldersTask));

}

bool FolderService::isCreating()
{
    return createFolderTask != nullptr
            && tasksListModel->contains(createFolderTask)
            && createFolderTask->getStatus() == Enums::TaskStatus::InProgress;
}

bool FolderService::isRemoving()
{
    return removeFoldersTask != nullptr
            && tasksListModel->contains(removeFoldersTask)
            && removeFoldersTask->getStatus() == Enums::TaskStatus::InProgress;
}

void FolderService::setRemoveFoldersTask(TaskListItem *newRemoveFoldersTask)
{
    removeFoldersTask = newRemoveFoldersTask;
    connect(removeFoldersTask, &TaskListItem::updated, this, &FolderService::removeFolderTaskWasUpdated);
    emit removingChanged();
}

void FolderService::createFolderTaskWasFinished()
{
    createFolderTask = nullptr;
}

FoldersListModel *FolderService::getListModel() const
{
    return listModel;
}

void FolderService::clear()
{
    qDebug() << "Clear decrypted folders list";
    decrypted = false;
    listModel->clear();
    if(isCreating()){
        disconnect(tokenService, &TokenService::refreshTokenSuccess, this, &FolderService::postFolderRefreshTokenSuccess);
        disconnect(tokenService, &TokenService::refreshTokenFail, this, &FolderService::postFolderRefreshTokenFailed);
        setSavingListItemFolder(nullptr);
    }
}

void FolderService::setSavingListItemFolder(FolderListItem *newSavingListItemFolder)
{
    if(newSavingListItemFolder != savingListItemFolder){
        savingListItemFolder = newSavingListItemFolder;
    }
}

void FolderService::cancelSavingListItemFolder()
{
    // TODO: remove specific list item
    listModel->removeLast();
    setSavingListItemFolder(nullptr);
}

void FolderService::postFolderRefreshTokenSuccess()
{
    disconnect(tokenService, &TokenService::refreshTokenSuccess, this, &FolderService::postFolderRefreshTokenSuccess);
    disconnect(tokenService, &TokenService::refreshTokenFail, this, &FolderService::postFolderRefreshTokenFailed);
    createFolderTask->setMessage("Saving");
    postFolderReply = api->postFolder(cryptoService->encrypt(savingListItemFolder->getName()), tokenService->getAccessToken());
    connect(postFolderReply, &QNetworkReply::finished, this, &FolderService::postFolderSuccess);
}

void FolderService::postFolderRefreshTokenFailed(QString reason)
{
    createFolderTask->fail(reason);
    disconnect(tokenService, &TokenService::refreshTokenSuccess, this, &FolderService::postFolderRefreshTokenSuccess);
    disconnect(tokenService, &TokenService::refreshTokenFail, this, &FolderService::postFolderRefreshTokenFailed);
    cancelSavingListItemFolder();
}

void FolderService::postFolderSuccess()
{
    setSavingListItemFolder(nullptr);

    if(postFolderReply->error() != QNetworkReply::NoError && postFolderReply->error() != QNetworkReply::ProtocolInvalidOperationError){
        createFolderTask->fail(Api::getFailedReplyMessage(postFolderReply));
        cancelSavingListItemFolder();
        return;
    }

    int httpCode = postFolderReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if(httpCode != 200 && httpCode != 400){
        createFolderTask->fail(QString("Unexpected http response code on POST folder: ").append(httpCode));
        cancelSavingListItemFolder();
        return;
    }

    QJsonDocument json = QJsonDocument::fromJson(postFolderReply->readAll());

    if(!json.isObject()){
        createFolderTask->fail("Root JSON item in POST folder response body is not object");
        cancelSavingListItemFolder();
        return;
    }

    if(httpCode == 400){
        qWarning() << json.toJson(QJsonDocument::Compact);
        createFolderTask->fail("Bad request");
        cancelSavingListItemFolder();
        return;
    }

    Folder folder = folderFactory->create(json.object(), tokenService->getUserIdFromToken());
    stateService->add(folder);
    listModel->removeLast();
    addListItemToModel(&folder);
    createFolderTask->success();
}
