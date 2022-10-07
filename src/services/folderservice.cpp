#include "folderservice.h"

FolderService::FolderService(StateService *stateService, CryptoService *cryptoService,
                             TokenService *tokenService,
                             ApiJsonDumper *apiJsonDumper,
                             Api *api, QObject *parent):
    QObject(parent),
    stateService(stateService),
    cryptoService(cryptoService),
    tokenService(tokenService),
    api(api)
{
    listModel = new FoldersListModel();
    locale = QLocale::system();
    savingListItemFolder = nullptr;
    folderFactory = new FolderFactory(apiJsonDumper);
}

void FolderService::addListItemToModel(Folder *folder)
{
    QDateTime revisionDate;
    QString isoRevisionDate;
    QString dateTimeFormat = locale.dateTimeFormat(QLocale::ShortFormat);
    FolderListItem listItem;
    listItem.setId(folder->getId());
    listItem.setName(cryptoService->decryptToUtf8(folder->getName()));
    // remove redundant part of the dateTime string
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
    FolderListItem folderListItem;
    folderListItem.setName(name);
    listModel->add(folderListItem);
    setSavingListItemFolder(&folderListItem);

    connect(tokenService, &TokenService::refreshTokenSuccess, this, &FolderService::postFolderRefreshTokenSuccess);
    connect(tokenService, &TokenService::refreshTokenFail, this, &FolderService::postFolderRefreshTokenFailed);
    tokenService->validateToken();
}

bool FolderService::getCreating()
{
    return savingListItemFolder != nullptr;
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
    if(getCreating()){
        disconnect(tokenService, &TokenService::refreshTokenSuccess, this, &FolderService::postFolderRefreshTokenSuccess);
        disconnect(tokenService, &TokenService::refreshTokenFail, this, &FolderService::postFolderRefreshTokenFailed);
        setSavingListItemFolder(nullptr);
    }
}

void FolderService::setSavingListItemFolder(FolderListItem *newSavingListItemFolder)
{
    if(newSavingListItemFolder != savingListItemFolder){
        savingListItemFolder = newSavingListItemFolder;
        emit creatingChanged();
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
    postFolderReply = api->postFolder(cryptoService->encrypt(savingListItemFolder->getName()), tokenService->getAccessToken());
    connect(postFolderReply, &QNetworkReply::finished, this, &FolderService::postFolderSuccess);
}

void FolderService::postFolderRefreshTokenFailed(QString reason)
{
    disconnect(tokenService, &TokenService::refreshTokenSuccess, this, &FolderService::postFolderRefreshTokenSuccess);
    disconnect(tokenService, &TokenService::refreshTokenFail, this, &FolderService::postFolderRefreshTokenFailed);
    cancelSavingListItemFolder();
    qWarning() << "Unable to POST folder: unable to refresh token:" << reason;
}

void FolderService::postFolderSuccess()
{
    setSavingListItemFolder(nullptr);

    if(postFolderReply->error() != QNetworkReply::NoError && postFolderReply->error() != QNetworkReply::ProtocolInvalidOperationError){
        qCritical() << "Post folder request failed. Error:" << postFolderReply->error();
        cancelSavingListItemFolder();
        return;
    }

    int httpCode = postFolderReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if(httpCode != 200 && httpCode != 400){
        qCritical() << "Unexpected http response code on POST folder:" << httpCode;
        cancelSavingListItemFolder();
        return;
    }

    QJsonDocument json = QJsonDocument::fromJson(postFolderReply->readAll());

    if(!json.isObject()){
        qCritical() << "Root JSON item in POST folder response body is not object";
        cancelSavingListItemFolder();
        return;
    }

    if(httpCode == 400){
        qWarning() << json.toJson(QJsonDocument::Compact);
        cancelSavingListItemFolder();
        return;
    }

    Folder folder = folderFactory->create(json.object(), tokenService->getUserIdFromToken());
    stateService->add(folder);
    listModel->removeLast();
    addListItemToModel(&folder);
}
