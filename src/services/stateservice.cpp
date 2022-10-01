#include "stateservice.h"

StateService::StateService(QObject *parent) :
    QObject(parent)
{
    folders = new QList<Folder>();
    ciphers = new QList<Cipher>();
}

void StateService::add(Folder &item)
{
    folders->append(item);
    emit foldersWereChanged();
}

void StateService::add(Cipher &item)
{
    ciphers->append(item);
    emit ciphersWereChanged();
}

QList<Folder> *StateService::getFolders() const
{
    return folders;
}

QList<Cipher> *StateService::getCiphers() const
{
    return ciphers;
}

void StateService::clear()
{
    qDebug() << "Clear all encrypted data from in-memory cache";
    folders->clear();
    emit foldersWereChanged();
    ciphers->clear();
    emit ciphersWereChanged();
}
