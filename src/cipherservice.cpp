#include "cipherservice.h"

CipherService::CipherService(CryptoService *cryptoService, CipherView *cipherView, QObject *parent) :
    QObject(parent),
    cryptoService(cryptoService),
    cipherView(cipherView)
{
    ciphers = new QList<Cipher>;
    ciphersListModel = new CiphersListModel();
}

void CipherService::decryptAll()
{
    ciphersListModel->clear();
    QList<Cipher>::iterator i;
    for(i = ciphers->begin(); i != ciphers->end(); i++){
        qDebug() << "decrypt" << i->getId();
        CipherListItem cipherListItem;
        cipherListItem.setId(i->getId());
        cipherListItem.setName(cryptoService->decryptToUtf8((*i).getName()));
        cipherListItem.setType((*i).getType());
        if(cipherListItem.getType() == Cipher::CipherType::Login){
            qDebug() << "username data" << (*i).getLogin()->getUsername().getData();
            qDebug() << "username mac" << (*i).getLogin()->getUsername().getMac();
            qDebug() << "username iv" << (*i).getLogin()->getUsername().getIv();
            cipherListItem.setSubtitle(cryptoService->decryptToUtf8((*i).getLogin()->getUsername()));
        }
        ciphersListModel->add(cipherListItem);
    }
}

void CipherService::display(QString id)
{
    QList<Cipher>::iterator i;
    for(i = ciphers->begin(); i != ciphers->end(); i++){
        if(i->getId() != id){
            continue;
        }
        cipherView->setName(cryptoService->decryptToUtf8(i->getName()));
        cipherView->setType(i->getType());
        if(i->getType() == Cipher::CipherType::Login){
            cipherView->setLoginUsername(cryptoService->decryptToUtf8(i->getLogin()->getUsername()));
        }
        return;
    }

    qWarning() << "cipher not found" << id;

}

int CipherService::getCount()
{
    return ciphers->count();
}

void CipherService::add(Cipher &item)
{
    qDebug() << "pre add cipher to list";
    ciphers->append(item);
    qDebug() << "post add cipher to list";
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
