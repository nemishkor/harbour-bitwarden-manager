#ifndef CIPHERSTRING_H
#define CIPHERSTRING_H

#include <QDebug>
#include <QString>
#include <QStringList>

class CipherString
{
public:
    CipherString();
    CipherString(QString encryptedString);
    enum EncryptionType {
        AesCbc256_B64 = 0,
        AesCbc128_HmacSha256_B64 = 1,
        AesCbc256_HmacSha256_B64 = 2,
        Rsa2048_OaepSha256_B64 = 3,
        Rsa2048_OaepSha1_B64 = 4,
        Rsa2048_OaepSha256_HmacSha256_B64 = 5,
        Rsa2048_OaepSha1_HmacSha256_B64 = 6,
    };
    CipherString(const CipherString& c) :
        encryptionType(c.getEncryptionType()),
        data(c.getData()),
        iv(c.getIv()),
        mac(c.getMac())
    {
        qDebug() << "copy cipherString";
    }

    void fill(QString encryptedString);

    EncryptionType getEncryptionType() const;

    QString getData() const;

    QString getIv() const;

    QString getMac() const;

private:
    EncryptionType encryptionType;
    QString data;
    QString iv;
    QString mac;

};

#endif // CIPHERSTRING_H
