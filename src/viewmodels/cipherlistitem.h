#ifndef DECRYPTEDCIPHER_H
#define DECRYPTEDCIPHER_H

#include <QString>

#include "src/models/cipher.h"

class CipherListItem
{
public:
    CipherListItem();

    QString getId() const;
    void setId(const QString &value);

    QString getName() const;
    void setName(const QString &value);

    void setType(const Cipher::CipherType &value);
    Cipher::CipherType getType() const;

    QString getSubtitle() const;
    void setSubtitle(const QString &value);

    bool getChecked() const;
    void setChecked(bool newChecked);

    bool getRemoving() const;
    void setRemoving(bool newRemoving);

private:
    QString id;
    QString name;
    QString subtitle;
    Cipher::CipherType type;
    bool checked = false;
    bool removing = false;
};

#endif // DECRYPTEDCIPHER_H
