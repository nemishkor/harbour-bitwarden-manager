#include "symmetriccryptokey.h"

SymmetricCryptoKey::SymmetricCryptoKey()
{
}

SymmetricCryptoKey::SymmetricCryptoKey(QByteArray key)
{
    fillWithKey(key);
}

SymmetricCryptoKey::SymmetricCryptoKey(QByteArray key, CipherString::EncryptionType encType)
{
    this->key = key;
    keyB64 = key.toBase64();
    this->encType = encType;
    if(encType == CipherString::EncryptionType::AesCbc256_B64 && key.length() == 32){
        encKey.append(key);
        macKey = nullptr;
    } else if(encType == CipherString::EncryptionType::AesCbc128_HmacSha256_B64 && key.length() == 32){
        encKey.append(key.mid(0, 16));
        macKey.append(key.mid(16, 16));
        macKeyB64 = macKey.toBase64();
    } else if(encType == CipherString::EncryptionType::AesCbc256_HmacSha256_B64 && key.length() == 64){
        encKey.append(key.mid(0, 32));
        macKey.append(key.mid(32, 32));
        macKeyB64 = macKey.toBase64();
    } else {
        qWarning() << "Unsupported encType/key length.";
        throw new QException();
    }
    encKeyB64 = encKey.toBase64();
}

void SymmetricCryptoKey::fillWithKey(QByteArray key)
{
    this->key = key;
    keyB64 = key.toBase64();
    qDebug() << "key length" << key.length();
    if(key.length() == 32){
        encType = CipherString::EncryptionType::AesCbc256_B64;
        encKey = key;
        macKey = nullptr;
    } else if (key.length() == 64){
        encType = CipherString::EncryptionType::AesCbc256_HmacSha256_B64;
        encKey.append(key.mid(0, 32));
        macKey.append(key.mid(32, 32));
        macKeyB64 = macKey.toBase64();
    } else {
        qWarning() << "Unable to determine encType.";
        throw new QException();
    }
    encKeyB64 = encKey.toBase64();
}

void SymmetricCryptoKey::clear()
{
    key.clear();
    encKey.clear();
    macKey.clear();
    keyB64.clear();
    encKeyB64.clear();
    macKeyB64.clear();
}

CipherString::EncryptionType SymmetricCryptoKey::getEncType() const
{
    return encType;
}

QByteArray SymmetricCryptoKey::getEncKey() const
{
    return encKey;
}

QByteArray SymmetricCryptoKey::getMacKey() const
{
    return macKey;
}

QByteArray SymmetricCryptoKey::getKey() const
{
    return key;
}
