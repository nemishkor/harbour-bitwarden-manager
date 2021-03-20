#ifndef DECRYPTEDCIPHER_H
#define DECRYPTEDCIPHER_H

#include <QString>
#include "cipher.h"

class CipherListItem
{
public:
    CipherListItem();

    QString getName() const;
    void setName(const QString &value);

    void setType(const Cipher::CipherType &value);
    Cipher::CipherType getType() const;

    QString getSubtitle() const;
    void setSubtitle(const QString &value);

private:
    QString name;
    QString subtitle;
    Cipher::CipherType type;
};

#endif // DECRYPTEDCIPHER_H
