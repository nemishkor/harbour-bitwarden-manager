#include "cipher.h"

Cipher::Cipher(CipherString name): name(name)
{
    fields = new QList<CipherField>();
    passwordHistory = new QList<CipherPasswordHistoryItem>();
}

Cipher::Cipher(const Cipher &cipher) :
    name(cipher.name),
    fields(cipher.fields),
    passwordHistory(cipher.passwordHistory)
{
    qDebug() << "copy cipher!";
}

Cipher::~Cipher()
{
//    delete fields;
//    delete passwordHistory;
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

CipherString Cipher::getName() const
{
    return name;
}

void Cipher::setName(const CipherString &value)
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

CipherLogin *Cipher::getLogin()
{
    return &login;
}

void Cipher::addField(CipherField cipherField)
{
    fields->append(cipherField);
}

QList<CipherField> *Cipher::getFields() const
{
    return fields;
}

CipherCard *Cipher::getCard()
{
    return &card;
}

CipherIdentity *Cipher::getIdentity()
{
    return &identity;
}

void Cipher::addPasswordHistoryItem(CipherPasswordHistoryItem passwordHistoryItem)
{
    passwordHistory->append(passwordHistoryItem);
}

QList<CipherPasswordHistoryItem> *Cipher::getPasswordHistory() const
{
    return passwordHistory;
}
