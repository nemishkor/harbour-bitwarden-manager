#include "cipherfactory.h"

CipherFactory::CipherFactory(ApiJsonDumper *apiJsonDumper):
    apiJsonDumper(apiJsonDumper)
{

}

Cipher CipherFactory::create(QJsonObject apiCipher, QString userId)
{
    apiJsonDumper->dumpCipherFields(&apiCipher);

    QJsonArray::const_iterator i, cipherChildArrIt;
    QJsonObject c, l, card, identity, cipherChildArrItem;
    QJsonArray fields, passwordHistory;

    Cipher cipher(CipherString(apiCipher["name"].toString()));
    cipher.setId(apiCipher["id"].toString());
    if(apiCipher["organizationId"].isString()) {
        cipher.setOrganizationId(apiCipher["organizationId"].toString());
    }
    if(apiCipher["folderId"].isString()) {
        cipher.setFolderId(apiCipher["folderId"].toString());
    }
    cipher.setUserId(userId);
    cipher.setEdit(apiCipher["edit"].toBool());
    cipher.setViewPassword(apiCipher["viewPassword"].toBool());
    cipher.setOrganizationUseTotp(apiCipher["organizationUseTotp"].toBool());
    cipher.setFavorite(apiCipher["favorite"].toBool());
    cipher.setRevisionDate(apiCipher["revisionDate"].toString());
    cipher.setType(static_cast<Cipher::CipherType>(apiCipher["type"].toInt()));
    cipher.setSizeName(apiCipher["sizeName"].toString());
    cipher.setNotes(apiCipher["notes"].toString());
    if(apiCipher["deletedDate"].isString()) {
        cipher.setDeletedDate(apiCipher["deletedDate"].toString());
    }

    if(apiCipher.contains("login") && apiCipher["login"].isObject()){
        l = apiCipher["login"].toObject();
        apiJsonDumper->dumpCipherLoginFields(&l);
        cipher.getLogin()->fillPassword(l["password"].toString());
        if(l["passwordRevisionDate"].isString()){
            cipher.getLogin()->setPasswordRevisionDate(l["passwordRevisionDate"].toString());
        }
        if(l["uri"].isString()) {
            cipher.getLogin()->fillUri(l["uri"].toString());
        }
        if(l["totp"].isString()) {
            cipher.getLogin()->fillTotp(l["totp"].toString());
        }
        cipher.getLogin()->fillUsername(l["username"].toString());
    }

    if(apiCipher.contains("card") && apiCipher["card"].isObject()){
        card = apiCipher["card"].toObject();
        apiJsonDumper->dumpCipherCardFields(&card);
        if(card["brand"].isString()){
            cipher.getCard()->fillBrand(card["brand"].toString());
        }
        if(card["cardholderName"].isString()){
            cipher.getCard()->fillCardholderName(card["cardholderName"].toString());
        }
        if(card["code"].isString()){
            cipher.getCard()->fillCode(card["code"].toString());
        }
        if(card["expMonth"].isString()){
            cipher.getCard()->fillExpMonth(card["expMonth"].toString());
        }
        if(card["expYear"].isString()){
            cipher.getCard()->fillExpYear(card["expYear"].toString());
        }
        if(card["number"].isString()){
            cipher.getCard()->fillNumber(card["number"].toString());
        }
    }

    if(apiCipher.contains("identity") && apiCipher["identity"].isObject()){
        qDebug() << "Sync identity cipher";
        identity = apiCipher["identity"].toObject();
        apiJsonDumper->dumpCipherIdentityFields(&identity);

        if(identity["address1"].isString()){
            cipher.getIdentity()->fillAddress1(identity["address1"].toString());
        }
        if(identity["address2"].isString()){
            cipher.getIdentity()->fillAddress2(identity["address1"].toString());
        }
        if(identity["address3"].isString()){
            cipher.getIdentity()->fillAddress3(identity["address3"].toString());
        }
        if(identity["city"].isString()){
            cipher.getIdentity()->fillCity(identity["city"].toString());
        }
        if(identity["company"].isString()){
            cipher.getIdentity()->fillCompany(identity["company"].toString());
        }
        if(identity["country"].isString()){
            cipher.getIdentity()->fillCountry(identity["country"].toString());
        }
        if(identity["email"].isString()){
            cipher.getIdentity()->fillEmail(identity["email"].toString());
        }
        if(identity["firstName"].isString()){
            cipher.getIdentity()->fillFirstName(identity["firstName"].toString());
        }
        if(identity["lastName"].isString()){
            cipher.getIdentity()->fillLastName(identity["lastName"].toString());
        }
        if(identity["licenseNumber"].isString()){
            cipher.getIdentity()->fillLicenseNumber(identity["licenseNumber"].toString());
        }
        if(identity["middleName"].isString()){
            cipher.getIdentity()->fillMiddleName(identity["middleName"].toString());
        }
        if(identity["passportNumber"].isString()){
            cipher.getIdentity()->fillPassportNumber(identity["passportNumber"].toString());
        }
        if(identity["phone"].isString()){
            cipher.getIdentity()->fillPhone(identity["phone"].toString());
        }
        if(identity["postalCode"].isString()){
            cipher.getIdentity()->fillPostalCode(identity["postalCode"].toString());
        }
        if(identity["ssn"].isString()){
            cipher.getIdentity()->fillSSN(identity["ssn"].toString());
        }
        if(identity["state"].isString()){
            cipher.getIdentity()->fillState(identity["state"].toString());
        }
        if(identity["title"].isString()){
            cipher.getIdentity()->fillTitle(identity["title"].toString());
        }
        if(identity["username"].isString()){
            cipher.getIdentity()->fillUsername(identity["username"].toString());
        }
        qDebug() << "Sync identity cipher finished";
    }

    fields = c["fields"].toArray();
    for(cipherChildArrIt = fields.constBegin(); cipherChildArrIt != fields.constEnd(); cipherChildArrIt++){
        cipherChildArrItem = (*cipherChildArrIt).toObject();
        cipher.addField(CipherField(
            CipherString(cipherChildArrItem["name"].toString()),
            static_cast<CipherField::FieldType>(cipherChildArrItem["type"].toInt()),
            CipherString(cipherChildArrItem["value"].toString())
        ));
    }

    if(apiCipher["passwordHistory"].isArray()){
        passwordHistory = c["passwordHistory"].toArray();
        for(cipherChildArrIt = passwordHistory.constBegin(); cipherChildArrIt != passwordHistory.constEnd(); cipherChildArrIt++){
            cipherChildArrItem = (*cipherChildArrIt).toObject();
            apiJsonDumper->dumpCipherPasswordHistoryFields(&cipherChildArrItem);
            cipher.addPasswordHistoryItem(CipherPasswordHistoryItem(
                cipherChildArrItem["lastUsedDate"].toString(),
                CipherString(cipherChildArrItem["password"].toString())
            ));
        }
    }

    return cipher;
}
