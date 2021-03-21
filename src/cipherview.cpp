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
