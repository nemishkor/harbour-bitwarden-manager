#include "cipherstring.h"

CipherString::CipherString()
{

}

CipherString::CipherString(QString encryptedString)
{
    fill(encryptedString);
}

QString CipherString::buildEncryptedString(SymmetricCryptoKey keyForEnc, QByteArray iv, QByteArray data)
{
    QByteArray ivBase64 = iv.toBase64(),
            dataBase64 = data.toBase64();
    QString encStr = QString::number((int) keyForEnc.getEncType()).append(".");

    if(!ivBase64.isNull()){
        encStr.append(ivBase64).append("|");
    }

    encStr.append(dataBase64);

    if(!keyForEnc.getMacKey().isNull()){
        qDebug() << "mac key is not null. We have to set MAC";
        QByteArray macData;
        macData.append(iv).append(data);
        QByteArray mac(QMessageAuthenticationCode::hash(macData, keyForEnc.getMacKey(), QCryptographicHash::Sha256));

        encStr.append("|").append(mac.toBase64());
    }

    return encStr;
}

Enums::EncryptionType CipherString::getEncryptionType() const
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

void CipherString::fill(QString encryptedString)
{
    if(encryptedString.isEmpty()){
        qWarning() << "encrypted string is empty";
    }
    QStringList headerPieses = encryptedString.split(".");
    QStringList encPieses;
    qDebug() << "header pieses length" << headerPieses.length();
    if(headerPieses.count() == 2){
        encryptionType = static_cast<Enums::EncryptionType>(headerPieses[0].toInt());
        encPieses = headerPieses[1].split("|");
    } else {
        encPieses = encryptedString.split("|");
        qDebug() << "enc pieses length" << encPieses.length();
        encryptionType = encPieses.length() == 3
                ? Enums::EncryptionType::AesCbc128_HmacSha256_B64
                : Enums::EncryptionType::AesCbc256_B64;
    }

    switch (encryptionType) {
    case Enums::EncryptionType::AesCbc128_HmacSha256_B64:
    case Enums::EncryptionType::AesCbc256_HmacSha256_B64:
        iv = encPieses[0];
        data = encPieses[1];
        mac = encPieses[2];
        break;
    case Enums::EncryptionType::AesCbc256_B64:
        iv = encPieses[0];
        data = encPieses[1];
        break;
    case Enums::EncryptionType::Rsa2048_OaepSha256_B64:
    case Enums::EncryptionType::Rsa2048_OaepSha1_B64:
        data = encPieses[0];
        break;
    default:
        break;
    }
    qDebug() << "encryption type" << encryptionType;
}

