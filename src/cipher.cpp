#include "cipher.h"

Cipher::Cipher()
{

}

QString Cipher::getId() const
{
    return id;
}

void Cipher::setId(const QString &value)
{
    id = value;
}

QString Cipher::getOrganizationId() const
{
    return organizationId;
}

void Cipher::setOrganizationId(const QString &value)
{
    organizationId = value;
}

QString Cipher::getFolderId() const
{
    return folderId;
}

void Cipher::setFolderId(const QString &value)
{
    folderId = value;
}

QString Cipher::getUserId() const
{
    return userId;
}

void Cipher::setUserId(const QString &value)
{
    userId = value;
}

bool Cipher::getEdit() const
{
    return edit;
}

void Cipher::setEdit(bool value)
{
    edit = value;
}

bool Cipher::getViewPassword() const
{
    return viewPassword;
}

void Cipher::setViewPassword(bool value)
{
    viewPassword = value;
}

bool Cipher::getOrganizationUseTotp() const
{
    return organizationUseTotp;
}

void Cipher::setOrganizationUseTotp(bool value)
{
    organizationUseTotp = value;
}

bool Cipher::getFavorite() const
{
    return favorite;
}

void Cipher::setFavorite(bool value)
{
    favorite = value;
}

QString Cipher::getRevisionDate() const
{
    return revisionDate;
}

void Cipher::setRevisionDate(const QString &value)
{
    revisionDate = value;
}

Cipher::CipherType Cipher::getType() const
{
    return type;
}

void Cipher::setType(const Cipher::CipherType &value)
{
    type = value;
}

QString Cipher::getSizeName() const
{
    return sizeName;
}

void Cipher::setSizeName(const QString &value)
{
    sizeName = value;
}

QString Cipher::getName() const
{
    return name;
}

void Cipher::setName(const QString &value)
{
    name = value;
}

QString Cipher::getNotes() const
{
    return notes;
}

void Cipher::setNotes(const QString &value)
{
    notes = value;
}

QString Cipher::getDeletedDate() const
{
    return deletedDate;
}

void Cipher::setDeletedDate(const QString &value)
{
    deletedDate = value;
}
