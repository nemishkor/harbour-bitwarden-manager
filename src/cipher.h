#ifndef CIPHER_H
#define CIPHER_H

#include <QString>

#include "cipherstring.h"

class Cipher
{
public:
    Cipher(CipherString name);
    enum CipherType {
        Login = 1,
        SecureNote = 2,
        Card = 3,
        Identity = 4,
    };

    QString getId() const;
    void setId(const QString &value);

    QString getOrganizationId() const;
    void setOrganizationId(const QString &value);

    QString getFolderId() const;
    void setFolderId(const QString &value);

    QString getUserId() const;
    void setUserId(const QString &value);

    bool getEdit() const;
    void setEdit(bool value);

    bool getViewPassword() const;
    void setViewPassword(bool value);

    bool getOrganizationUseTotp() const;
    void setOrganizationUseTotp(bool value);

    bool getFavorite() const;
    void setFavorite(bool value);

    QString getRevisionDate() const;
    void setRevisionDate(const QString &value);

    CipherType getType() const;
    void setType(const CipherType &value);

    QString getSizeName() const;
    void setSizeName(const QString &value);

    CipherString getName() const;
    void setName(const CipherString &value);

    QString getNotes() const;
    void setNotes(const QString &value);

    QString getDeletedDate() const;
    void setDeletedDate(const QString &value);

private:
    QString id;
    QString organizationId;
    QString folderId;
    QString userId;
    bool edit;
    bool viewPassword;
    bool organizationUseTotp;
    bool favorite;
    QString revisionDate;
    CipherType type;
    QString sizeName;
    CipherString name;
    QString notes;
    QString deletedDate;
};

#endif // CIPHER_H
