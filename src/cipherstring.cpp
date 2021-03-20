#include "cipherstring.h"

CipherString::CipherString(QString encryptedString):
    encryptedString(encryptedString)
{
    qDebug() << "create cipher string";
    QStringList headerPieses = encryptedString.split(".");
    QStringList encPieses;
    qDebug() << "header pieses length" << headerPieses.length();
    if(headerPieses.count() == 2){
        encryptionType = static_cast<EncryptionType>(headerPieses[0].toInt());
        encPieses = headerPieses[1].split("|");
    } else {
        encPieses = encryptedString.split("|");
        qDebug() << "enc pieses length" << encPieses.length();
        encryptionType = encPieses.length() == 3
                ? EncryptionType::AesCbc128_HmacSha256_B64
                : EncryptionType::AesCbc256_B64;
    }

    switch (encryptionType) {
    case EncryptionType::AesCbc128_HmacSha256_B64:
    case EncryptionType::AesCbc256_HmacSha256_B64:
        iv = encPieses[0];
        data = encPieses[1];
        mac = encPieses[2];
        break;
    case EncryptionType::AesCbc256_B64:
        iv = encPieses[0];
        data = encPieses[1];
        break;
    case EncryptionType::Rsa2048_OaepSha256_B64:
    case EncryptionType::Rsa2048_OaepSha1_B64:
        data = encPieses[0];
        break;
    default:
        break;
    }
    qDebug() << "encryption type" << encryptionType;
}

CipherString::EncryptionType CipherString::getEncryptionType() const
{
    return encryptionType;
}

QString CipherString::getData() const
{
    return data;
}

QString CipherString::getIv() const
{
    return iv;
}

QString CipherString::getMac() const
{
    return mac;
}

