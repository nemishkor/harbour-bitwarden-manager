#include "folderservice.h"

FolderService::FolderService(StateService *stateService, CryptoService *cryptoService, QObject *parent):
    QObject(parent),
    stateService(stateService),
    cryptoService(cryptoService)
{
    listModel = new FoldersListModel();
    locale = QLocale::system();
}

void FolderService::decryptAll()
{
    if(decrypted){
        qDebug() << "Folders are already decrypted. Using in-memory cache";
        return;
    }
    QDateTime revisionDate;
    QString isoRevisionDate;
    QString dateTimeFormat = locale.dateTimeFormat(QLocale::ShortFormat);
    QTimeZone systemTimeZone = QTimeZone::systemTimeZone();
    qDebug() << "systemTimeZone.country" << systemTimeZone.country();
    QList<Folder>::iterator i;
    QList<Folder> *folders = stateService->getFolders();
    for(i = folders->begin(); i != folders->end(); i++){
        FolderListItem listItem;
        listItem.setId(i->getId());
        listItem.setName(cryptoService->decryptToUtf8(i->getName()));
        // remove redundant part of the dateTime string
        // Bitwarden returns as example:
        // 2022-09-30T20:23:23.2123432Z
        // We need only "2022-09-30T20:23:23" part
        isoRevisionDate = i->getRevisionDate().mid(0, 19);
        revisionDate = QDateTime::fromString(isoRevisionDate, Qt::ISODate);
        // Bitwarden returns time in UTC
        revisionDate.setTimeSpec(Qt::UTC);
        listItem.setRevisionDate(revisionDate.toLocalTime().toString(dateTimeFormat));
        listModel->add(listItem);
    }
    qDebug() << "Folders were decrypted";
    decrypted = true;
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
}
