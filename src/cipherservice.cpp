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

void CipherService::decryptAll(bool deletedOnly)
{
    ciphersListModel->clear();
    QList<Cipher>::iterator i;
    for(i = ciphers->begin(); i != ciphers->end(); i++){
        if(deletedOnly && i->getDeletedDate().isNull()){
            continue;
        }
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

        if(i->getType() == Cipher::CipherType::Identity){
            qDebug() << "decrypt identity address1";
            cipherView->setIdentityAddress1(cryptoService->decryptToUtf8(i->getIdentity()->getAddress1()));
            qDebug() << "decrypt identity address2";
            cipherView->setIdentityAddress2(cryptoService->decryptToUtf8(i->getIdentity()->getAddress2()));
            qDebug() << "decrypt identity address3";
            cipherView->setIdentityAddress3(cryptoService->decryptToUtf8(i->getIdentity()->getAddress3()));
            qDebug() << "decrypt identity city";
            cipherView->setIdentityCity(cryptoService->decryptToUtf8(i->getIdentity()->getCity()));
            qDebug() << "decrypt identity company";
            cipherView->setIdentityCompany(cryptoService->decryptToUtf8(i->getIdentity()->getCompany()));
            qDebug() << "decrypt identity country";
            cipherView->setIdentityCountry(cryptoService->decryptToUtf8(i->getIdentity()->getCountry()));
            qDebug() << "decrypt identity email";
            cipherView->setIdentityEmail(cryptoService->decryptToUtf8(i->getIdentity()->getEmail()));
            qDebug() << "decrypt identity first name";
            cipherView->setIdentityFirstName(cryptoService->decryptToUtf8(i->getIdentity()->getFirstName()));
            qDebug() << "decrypt identity last name";
            cipherView->setIdentityLastName(cryptoService->decryptToUtf8(i->getIdentity()->getLastName()));
            qDebug() << "decrypt identity license number";
            cipherView->setIdentityLicenseNumber(cryptoService->decryptToUtf8(i->getIdentity()->getLicenseNumber()));
            qDebug() << "decrypt identity middle name";
            cipherView->setIdentityMiddleName(cryptoService->decryptToUtf8(i->getIdentity()->getMiddleName()));
            qDebug() << "decrypt identity password number";
            cipherView->setIdentityPassportNumber(cryptoService->decryptToUtf8(i->getIdentity()->getPassportNumber()));
            qDebug() << "decrypt identity phone";
            cipherView->setIdentityPhone(cryptoService->decryptToUtf8(i->getIdentity()->getPhone()));
            qDebug() << "decrypt identity postal code";
            cipherView->setIdentityPostalCode(cryptoService->decryptToUtf8(i->getIdentity()->getPostalCode()));
            qDebug() << "decrypt identity SSN";
            cipherView->setIdentitySSN(cryptoService->decryptToUtf8(i->getIdentity()->getSSN()));
            qDebug() << "decrypt identity state";
            cipherView->setIdentityState(cryptoService->decryptToUtf8(i->getIdentity()->getState()));
            qDebug() << "decrypt identity title";
            cipherView->setIdentityTitle(cryptoService->decryptToUtf8(i->getIdentity()->getTitle()));
            qDebug() << "decrypt identity username";
            cipherView->setIdentityUsername(cryptoService->decryptToUtf8(i->getIdentity()->getUsername()));
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

int CipherService::getCountDeleted()
{
    int c = ciphers->count();
    QList<Cipher>::iterator i;
    for(i = ciphers->begin(); i != ciphers->end(); i++){
        if(i->getDeletedDate().isEmpty()){
            c--;
        }
    }
    return c;
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
