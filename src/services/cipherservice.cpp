#include "cipherservice.h"

CipherService::CipherService(StateService *stateService,
                             CryptoService *cryptoService,
                             CipherView *cipherView,
                             QObject *parent) :
    QObject(parent),
    stateService(stateService),
    cryptoService(cryptoService),
    cipherView(cipherView)
{
    ciphersListModel = new CiphersListModel(this);
    cipherFieldsListModel = new CipherFieldsListModel(this);
    cipherPasswordHistoryListModel = new CipherPasswordHistoryListModel(this);
}

void CipherService::decryptAll(bool deletedOnly, QString folderId)
{
    filters = CipherFilters(deletedOnly, folderId);
    ciphersListModel->clear();
    QList<Cipher> *ciphers = stateService->getCiphers();
    QList<Cipher>::iterator i;
    for(i = ciphers->begin(); i != ciphers->end(); i++){
        if(!filters.filter(&*i)){
            continue;
        }
        ciphersListModel->add(decrypt(&*i));
    }
}

bool CipherService::decryptOne(const QString id, const QModelIndex index)
{
    QList<Cipher> *ciphers = stateService->getCiphers();
    QList<Cipher>::iterator i;
    for(i = ciphers->begin(); i != ciphers->end(); i++){
        if(!filters.filter(&*i) || i->getId() != id){
            continue;
        }

        if(index.isValid()){
            ciphersListModel->insert(decrypt(&*i), index.row());
        } else {
            ciphersListModel->add(decrypt(&*i));
        }

        return true;
    }

    return false;
}

void CipherService::display(QString id)
{
    qDebug() << "display cipher" << id;
    QList<Cipher> *ciphers = stateService->getCiphers();
    QList<Cipher>::iterator i;
    QList<CipherField>::const_iterator fields_i;
    QList<CipherPasswordHistoryItem>::const_iterator passwordHistoryIt;
    for(i = ciphers->begin(); i != ciphers->end(); i++){
        if(i->getId() != id){
            continue;
        }
        qDebug() << "decrypt name";
        cipherView->setName(cryptoService->decryptToUtf8(i->getName()));
        cipherView->setType(i->getType());
        cipherView->setRevisionDate(i->getRevisionDate());
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

        cipherPasswordHistoryListModel->clear();
        for(passwordHistoryIt = i->getPasswordHistory()->constBegin(); passwordHistoryIt != i->getPasswordHistory()->constEnd(); passwordHistoryIt++){
            qDebug() << "decrypt password history";
            cipherPasswordHistoryListModel->add(CipherPasswordHistoryListItem(
                passwordHistoryIt->getLastUsedDate(),
                cryptoService->decryptToUtf8(passwordHistoryIt->getPassword())
            ));
        }

        return;
    }

    qWarning() << "cipher not found" << id;

}

void CipherService::clear()
{
    ciphersListModel->clear();
}

QModelIndex CipherService::removeFromView(QString id)
{
    QModelIndexList list = ciphersListModel->match(ciphersListModel->index(0,0), CiphersListModel::IdRole, id);
    qDebug() << "Found" << list.count() << "ciphers in the model by id";
    if(list.count() != 1){
        qWarning() << "Expected exactly one cipher with id" << id << "but found" << list.count();
        return QModelIndex();
    }
    for(QModelIndex i : list){
        qDebug() << "Remove cipher at row" << i.row();
        bool removed = ciphersListModel->removeRow(i.row());
        if(!removed)
            qWarning() << "Unable to remove cipher at row" << i.row();
        return i;
    }
    return QModelIndex();
}

bool CipherService::removeFromState(QString id)
{
    QList<Cipher> *ciphers = stateService->getCiphers();
    int count = ciphers->count();
    for(int i = 0; i < count; i++){
        if(ciphers->at(i).getId() != id){
            continue;
        }
        ciphers->removeAt(i);
        return true;
    }
    return false;
}

CiphersListModel *CipherService::getCiphersListModel() const
{
    return ciphersListModel;
}

CipherFieldsListModel *CipherService::getCipherFieldsListModel() const
{
    return cipherFieldsListModel;
}

CipherPasswordHistoryListModel *CipherService::getCipherPasswordHistoryListModel() const
{
    return cipherPasswordHistoryListModel;
}

CipherListItem CipherService::decrypt(Cipher *cipher)
{
    CipherListItem cipherListItem;
    cipherListItem.setId(cipher->getId());
    cipherListItem.setName(cryptoService->decryptToUtf8(cipher->getName()));
    cipherListItem.setType(cipher->getType());
    if(cipherListItem.getType() == Cipher::CipherType::Login){
        cipherListItem.setSubtitle(cryptoService->decryptToUtf8(cipher->getLogin()->getUsername()));
    }
    return cipherListItem;
}
