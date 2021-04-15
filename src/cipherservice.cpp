#include "cipherservice.h"

CipherService::CipherService(CryptoService *cryptoService, CipherView *cipherView, QObject *parent) :
    QObject(parent),
    cryptoService(cryptoService),
    cipherView(cipherView)
{
    ciphers = new QList<Cipher>;
    ciphersListModel = new CiphersListModel();
    cipherFieldsListModel = new CipherFieldsListModel();
}

void CipherService::decryptAll()
{
    ciphersListModel->clear();
    QList<Cipher>::iterator i;
    for(i = ciphers->begin(); i != ciphers->end(); i++){
        CipherListItem cipherListItem;
        cipherListItem.setId(i->getId());
        cipherListItem.setName(cryptoService->decryptToUtf8((*i).getName()));
        cipherListItem.setType((*i).getType());
        if(cipherListItem.getType() == Cipher::CipherType::Login){
            cipherListItem.setSubtitle(cryptoService->decryptToUtf8((*i).getLogin()->getUsername()));
        }
        ciphersListModel->add(cipherListItem);
    }
}

void CipherService::display(QString id)
{
    qDebug() << "display cipher" << id;
    QList<Cipher>::iterator i;
    QList<CipherField>::const_iterator fields_i;
    for(i = ciphers->begin(); i != ciphers->end(); i++){
        if(i->getId() != id){
            continue;
        }
        qDebug() << "decrypt name";
        cipherView->setName(cryptoService->decryptToUtf8(i->getName()));
        cipherView->setType(i->getType());
        qDebug() << "decrypt notes";
        cipherView->setNotes(cryptoService->decryptToUtf8(i->getNotes()));
        qDebug() << "Deleteddate" << i->getDeletedDate();
        cipherView->setDeletedDate(i->getDeletedDate());

        if(i->getType() == Cipher::CipherType::Login){
            qDebug() << "decrypt login username";
            cipherView->setLoginUsername(cryptoService->decryptToUtf8(i->getLogin()->getUsername()));
            cipherView->setLoginPasswordRevisionDate(i->getLogin()->getPasswordRevisionDate());
            qDebug() << "decrypt login password";
            cipherView->setLoginPassword(cryptoService->decryptToUtf8(i->getLogin()->getPassword()));
            qDebug() << "decrypt login uri";
            cipherView->setLoginUri(cryptoService->decryptToUtf8(i->getLogin()->getUri()));
            qDebug() << "decrypt login totp";
            cipherView->setLoginTotp(cryptoService->decryptToUtf8(i->getLogin()->getTotp()));
        }

        if(i->getType() == Cipher::CipherType::Card){
            qDebug() << "decrypt card brand";
            cipherView->setCardBrand(cryptoService->decryptToUtf8(i->getCard()->getBrand()));
            qDebug() << "decrypt card cardholder";
            cipherView->setCardCardholder(cryptoService->decryptToUtf8(i->getCard()->getCardholderName()));
            qDebug() << "decrypt card code";
            cipherView->setCardCode(cryptoService->decryptToUtf8(i->getCard()->getCode()));
            qDebug() << "decrypt card expMonth";
            cipherView->setCardExpMonth(cryptoService->decryptToUtf8(i->getCard()->getExpMonth()));
            qDebug() << "decrypt card expYear";
            cipherView->setCardExpYear(cryptoService->decryptToUtf8(i->getCard()->getExpYear()));
            qDebug() << "decrypt card number";
            cipherView->setCardNumber(cryptoService->decryptToUtf8(i->getCard()->getNumber()));
        }

        cipherFieldsListModel->clear();
        for(fields_i = i->getFields()->constBegin(); fields_i != i->getFields()->constEnd(); fields_i++){
            qDebug() << "decrypt fields";
            cipherFieldsListModel->add(CipherFieldListItem(
                cryptoService->decryptToUtf8(fields_i->getName()),
                fields_i->getType(),
                cryptoService->decryptToUtf8(fields_i->getValue())
            ));
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

CipherFieldsListModel *CipherService::getCipherFieldsListModel() const
{
    return cipherFieldsListModel;
}
