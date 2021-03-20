#include "cipherservice.h"

CipherService::CipherService(QObject *parent) : QObject(parent)
{
    ciphers = new QList<Cipher>;
    ciphersListModel = new CiphersListModel();
}

void CipherService::decryptAll()
{
    ciphersListModel->clear();
    QList<Cipher>::const_iterator i;
    for(i = ciphers->constBegin(); i != ciphers->constEnd(); i++){
        CipherListItem cipherListItem;
//        cipherListItem.setName()
    }
}

int CipherService::getCount()
{
    return ciphers->count();
}

void CipherService::add(Cipher &item)
{
    ciphers->append(item);
    emit countChanged();
}

void CipherService::clear()
{
    ciphers->clear();
    emit countChanged();
}

CiphersListModel *CipherService::getCiphersListModel() const
{
    return ciphersListModel;
}
