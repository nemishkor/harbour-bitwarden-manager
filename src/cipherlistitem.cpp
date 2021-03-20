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

void CipherListItem::setType(const Cipher::CipherType &value)
{
    type = value;
}

Cipher::CipherType CipherListItem::getType() const
{
    return type;
}

QString CipherListItem::getSubtitle() const
{
    return subtitle;
}

void CipherListItem::setSubtitle(const QString &value)
{
    subtitle = value;
}
