#include "cipherview.h"

CipherView::CipherView(QObject *parent) : QObject(parent)
{

}

QString CipherView::getName() const
{
    return name;
}

void CipherView::setName(const QString &value)
{
    name = value;
    emit nameChanged();
}

int CipherView::getType() const
{
    return (int) type;
}

void CipherView::setType(const Cipher::CipherType &value)
{
    type = value;
    emit typeChanged();
}

QString CipherView::getLoginUsername() const
{
    return loginUsername;
}

void CipherView::setLoginUsername(const QString &value)
{
    loginUsername = value;
    emit loginUsernameChanged();
}

QString CipherView::getLoginPasswordRevisionDate() const
{
    return loginPasswordRevisionDate;
}

void CipherView::setLoginPasswordRevisionDate(const QString &value)
{
    loginPasswordRevisionDate = value;
    emit loginPasswordRevisionDateChanged();
}

QString CipherView::getLoginPassword() const
{
    return loginPassword;
}

void CipherView::setLoginPassword(const QString &value)
{
    loginPassword = value;
    emit loginPasswordChanged();
}

QString CipherView::getLoginUri() const
{
    return loginUri;
}

void CipherView::setLoginUri(const QString &value)
{
    loginUri = value;
    emit loginUriChanged();
}

QString CipherView::getNotes() const
{
    return notes;
}

void CipherView::setNotes(const QString &value)
{
    notes = value;
    emit notesChanged();
}

bool CipherView::getFavorite() const
{
    return favorite;
}

void CipherView::setFavorite(bool value)
{
    favorite = value;
    emit favoriteChanged();
}

QString CipherView::getLoginTotp() const
{
    return loginTotp;
}

void CipherView::setLoginTotp(const QString &value)
{
    loginTotp = value;
    emit loginTotpChanged();
}

QString CipherView::getCardBrand() const
{
    return cardBrand;
}

void CipherView::setCardBrand(const QString &value)
{
    if(cardBrand == value){
        return;
    }
    cardBrand = value;
    emit cardBrandChanged();
}

QString CipherView::getCardCardholder() const
{
    return cardCardholder;
}

void CipherView::setCardCardholder(const QString &value)
{
    if(cardCardholder == value){
        return;
    }
    cardCardholder = value;
    emit cardCardholderChanged();
}

QString CipherView::getCardCode() const
{
    return cardCode;
}

void CipherView::setCardCode(const QString &value)
{
    if(cardCode == value){
        return;
    }
    cardCode = value;
    emit cardCodeChanged();
}

QString CipherView::getCardExpMonth() const
{
    return cardExpMonth;
}

void CipherView::setCardExpMonth(const QString &value)
{
    if(cardExpMonth == value){
        return;
    }
    cardExpMonth = value;
    emit cardExpMonthChanged();
}

QString CipherView::getCardExpYear() const
{
    return cardExpYear;
}

void CipherView::setCardExpYear(const QString &value)
{
    if(cardExpYear == value){
        return;
    }
    cardExpYear = value;
    emit cardExpYearChanged();
}

QString CipherView::getCardNumber() const
{
    return cardNumber;
}

void CipherView::setCardNumber(const QString &value)
{
    if(cardNumber == value){
        return;
    }
    cardNumber = value;
    emit cardNumberChanged();
}

QString CipherView::getDeletedDate() const
{
    return deletedDate;
}

void CipherView::setDeletedDate(const QString &value)
{
    if(deletedDate == value){
        return;
    }
    deletedDate = value;
    emit deletedDateChanged();
}

QString CipherView::getIdentityAddress1() const
{
    return identityAddress1;
}

void CipherView::setIdentityAddress1(const QString &value)
{
    if(identityAddress1 == value)
        return;
    identityAddress1 = value;
    emit identityAddress1Changed();
}

QString CipherView::getIdentityAddress2() const
{
    return identityAddress2;
}

void CipherView::setIdentityAddress2(const QString &value)
{
    if(identityAddress2 == value)
        return;
    identityAddress2 = value;
    emit identityAddress2Changed();
}

QString CipherView::getIdentityAddress3() const
{
    return identityAddress3;
}

void CipherView::setIdentityAddress3(const QString &value)
{
    if(identityAddress3 == value)
        return;
    identityAddress3 = value;
    emit identityAddress3Changed();
}

QString CipherView::getIdentityCity() const
{
    return identityCity;
}

void CipherView::setIdentityCity(const QString &value)
{
    if(identityCity == value)
        return;
    identityCity = value;
    emit identityCityChanged();
}

QString CipherView::getIdentityCompany() const
{
    return identityCompany;
}

void CipherView::setIdentityCompany(const QString &value)
{
    if(identityCompany == value)
        return;
    identityCompany = value;
    emit identityCompanyChanged();
}

QString CipherView::getIdentityCountry() const
{
    return identityCountry;
}

void CipherView::setIdentityCountry(const QString &value)
{
    if(identityCountry == value)
        return;
    identityCountry = value;
    emit identityCountryChanged();
}

QString CipherView::getIdentityEmail() const
{
    return identityEmail;
}

void CipherView::setIdentityEmail(const QString &value)
{
    if(identityEmail == value)
        return;
    identityEmail = value;
    emit identityEmailChanged();
}

QString CipherView::getIdentityFirstName() const
{
    return identityFirstName;
}

void CipherView::setIdentityFirstName(const QString &value)
{
    if(identityFirstName == value)
        return;
    identityFirstName = value;
    emit identityFirstNameChanged();
}

QString CipherView::getIdentityLastName() const
{
    return identityLastName;
}

void CipherView::setIdentityLastName(const QString &value)
{
    if(identityLastName == value)
        return;
    identityLastName = value;
    emit identityLastNameChanged();
}

QString CipherView::getIdentityLicenseNumber() const
{
    return identityLicenseNumber;
}

void CipherView::setIdentityLicenseNumber(const QString &value)
{
    if(identityLicenseNumber == value)
        return;
    identityLicenseNumber = value;
    emit identityLicenseNumberChanged();
}

QString CipherView::getIdentityMiddleName() const
{
    return identityMiddleName;
}

void CipherView::setIdentityMiddleName(const QString &value)
{
    if(identityMiddleName == value)
        return;
    identityMiddleName = value;
    emit identityMiddleNameChanged();
}

QString CipherView::getIdentityPassportNumber() const
{
    return identityPassportNumber;
}

void CipherView::setIdentityPassportNumber(const QString &value)
{
    if(identityPassportNumber == value)
        return;
    identityPassportNumber = value;
    emit identityPassportNumberChanged();
}

QString CipherView::getIdentityPhone() const
{
    return identityPhone;
}

void CipherView::setIdentityPhone(const QString &value)
{
    if(identityPhone == value)
        return;
    identityPhone = value;
    emit identityPhoneChanged();
}

QString CipherView::getIdentityPostalCode() const
{
    return identityPostalCode;
}

void CipherView::setIdentityPostalCode(const QString &value)
{
    if(identityPostalCode == value)
        return;
    identityPostalCode = value;
    emit identityPostalCodeChanged();
}

QString CipherView::getIdentitySSN() const
{
    return identitySSN;
}

void CipherView::setIdentitySSN(const QString &value)
{
    if(identitySSN == value)
        return;
    identitySSN = value;
    emit identitySSNChanged();
}

QString CipherView::getIdentityState() const
{
    return identityState;
}

void CipherView::setIdentityState(const QString &value)
{
    if(identityState == value)
        return;
    identityState = value;
    emit identityStateChanged();
}

QString CipherView::getIdentityTitle() const
{
    return identityTitle;
}

void CipherView::setIdentityTitle(const QString &value)
{
    if(identityTitle == value)
        return;
    identityTitle = value;
    emit identityTitleChanged();
}

QString CipherView::getIdentityUsername() const
{
    return identityUsername;
}

void CipherView::setIdentityUsername(const QString &value)
{
    if(identityUsername == value)
        return;
    identityUsername = value;
    emit identityUsernameChanged();
}
