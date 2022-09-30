#include "stateservice.h"

StateService::StateService(QObject *parent) :
    QObject(parent)
{
    folders = new QList<Folder>();
}

void StateService::add(Folder &item)
{
    folders->append(item);
    emit foldersWereChanged();
}

QList<Folder> *StateService::getFolders() const
{
    return folders;
}

void StateService::clear()
{
    qDebug() << "Clear all encrypted data from in-memory cache";
    folders->clear();
    emit foldersWereChanged();
}
