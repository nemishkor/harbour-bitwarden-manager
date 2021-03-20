#ifndef SYMMETRICCRYPTOKEY_H
#define SYMMETRICCRYPTOKEY_H

#include <QByteArray>
#include <QString>
#include <QException>

#include "cipherstring.h"

class SymmetricCryptoKey
{
public:
    SymmetricCryptoKey();
    SymmetricCryptoKey(QByteArray key);
    SymmetricCryptoKey(QByteArray key, CipherString::EncryptionType encType);

    void fillWithKey(QByteArray key);
    void clear();

    QByteArray getKey() const;

    CipherString::EncryptionType getEncType() const;

    QByteArray getEncKey() const;

    QByteArray getMacKey() const;

private:
    QByteArray key;
    QByteArray encKey;
    QByteArray macKey;
    CipherString::EncryptionType encType;

    QString keyB64;
    QString encKeyB64;
    QString macKeyB64;

};

#endif // SYMMETRICCRYPTOKEY_H
