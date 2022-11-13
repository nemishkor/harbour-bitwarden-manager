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

bool CipherListItem::getChecked() const
{
    return checked;
}

void CipherListItem::setChecked(bool newChecked)
{
    checked = newChecked;
}

bool CipherListItem::getRemoving() const
{
    return removing;
}

void CipherListItem::setRemoving(bool newRemoving)
{
    removing = newRemoving;
}

QString CipherListItem::getId() const
{
    return id;
}

void CipherListItem::setId(const QString &value)
{
    id = value;
}
