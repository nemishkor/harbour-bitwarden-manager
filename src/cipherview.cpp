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
