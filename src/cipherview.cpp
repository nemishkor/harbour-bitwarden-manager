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
