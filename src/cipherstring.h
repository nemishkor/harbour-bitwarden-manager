#ifndef CIPHERSTRING_H
#define CIPHERSTRING_H

#include "symmetriccryptokey.h"
#include "enums.h"

#include <QDebug>
#include <QException>
#include <QString>
#include <QStringList>
#include <QMessageAuthenticationCode>

class CipherString
{
public:
    CipherString();
    CipherString(QString encryptedString);
    CipherString(const CipherString& c) :
        encryptionType(c.getEncryptionType()),
        data(c.getData()),
        iv(c.getIv()),
        mac(c.getMac())
    {
        qDebug() << "copy cipherString";
    }

    static QString buildEncryptedString(SymmetricCryptoKey keyForEnc, QByteArray iv, QByteArray data);

    void fill(QString encryptedString);

    Enums::EncryptionType getEncryptionType() const;

    QString getData() const;

    QString getIv() const;

    QString getMac() const;

private:
    Enums::EncryptionType encryptionType;
    QString data;
    QString iv;
    QString mac;

};

#endif // CIPHERSTRING_H
