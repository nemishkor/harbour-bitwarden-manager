#include "cipherlistitem.h"

CipherListItem::CipherListItem()
{

}

QString CipherListItem::getName() const
{
    return name;
}

void CipherListItem::setName(const QString &value)
{
    name = value;
}

QString CipherListItem::getLoginUsername() const
{
    return loginUsername;
}

void CipherListItem::setLoginUsername(const QString &value)
{
    loginUsername = value;
}
