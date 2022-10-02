#ifndef SYMMETRICCRYPTOKEY_H
#define SYMMETRICCRYPTOKEY_H

#include "enums.h"

#include <QByteArray>
#include <QString>
#include <QException>
#include <QDebug>

class SymmetricCryptoKey
{
public:
    SymmetricCryptoKey();
    SymmetricCryptoKey(QByteArray key);
    SymmetricCryptoKey(QByteArray key, Enums::EncryptionType encType);

    void fillWithKey(QByteArray key);
    void clear();

    QByteArray getKey() const;

    Enums::EncryptionType getEncType() const;

    QByteArray getEncKey() const;

    QByteArray getMacKey() const;

private:
    QByteArray key;
    QByteArray encKey;
    QByteArray macKey;
    Enums::EncryptionType encType;

    QString keyB64;
    QString encKeyB64;
    QString macKeyB64;

};

#endif // SYMMETRICCRYPTOKEY_H
