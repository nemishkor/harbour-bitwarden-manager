#include "cryptoservice.h"

CryptoService::CryptoService(QSettings *settings):
    QObject(nullptr),
    settings(settings)
{
    if(settings->contains("encKey")){
        qDebug() << "found encKey in settings";
        encKey = settings->value("encKey").toString();
    }
    if(settings->contains("hashedPassword")){
        qDebug() << "found hashedPassword in settings";
        hashedPassword = settings->value("hashedPassword").toString();
    }
}

QByteArray CryptoService::makeKey(QString password, QString email, KdfType kdf, int kdfIterations)
{
    if (kdfIterations < 5000) {
        qWarning("PBKDF2 iteration minimum is 5000.");
    }
    if(kdf != KdfType::PBKDF2_SHA256){
        qWarning("Unknown kdf type");
    }

    return pbkdf2(password.toLocal8Bit(), email, kdfIterations);
}

QString CryptoService::hashPassword(QByteArray key, QString password)
{
    return pbkdf2(key, password, 1).toBase64();
}

void CryptoService::setKey(const QString &value)
{
    if(key != value){
        key = value;
        if(key == "") {
            qDebug() << "key was deleted. Lock state";
        } else {
            qDebug() << "key was installed. Unlock state";
        }
        emit keyChanged();
    }
}

void CryptoService::setHashedPassword(const QString &value)
{
    hashedPassword = value;
    settings->setValue("hashedPassword", hashedPassword);
    settings->sync();
}

QString CryptoService::getEncKey() const
{
    return encKey;
}

void CryptoService::setEncKey(const QString &value)
{
    encKey = value;
    // TODO: remove saving encKey using QSettings ???
    settings->setValue("encKey", encKey);
    settings->sync();
}

QString CryptoService::getPrivateKey() const
{
    return privateKey;
}

void CryptoService::setPrivateKey(const QString &value)
{
    privateKey = value;
}

QString CryptoService::getKey() const
{
    return key;
}

QString CryptoService::getHashedPassword() const
{
    return hashedPassword;
}

void CryptoService::clearKey()
{
    setKey("");
}

void CryptoService::clearKeyPair()
{
    setPrivateKey("");
    // TODO: add public key ???
    //    setPublicKey("");
}

void CryptoService::clearEncKey()
{
    setEncKey("");
}

QByteArray CryptoService::pbkdf2(QByteArray passwordByteArray, QString saltString, int iterations)
{
    qDebug() << "pbkdf2 iterations:" << iterations;

    // cast
    QByteArray saltByteArray = saltString.toLocal8Bit();
    const char *password = passwordByteArray.constData();
    const unsigned char* salt = reinterpret_cast<const unsigned char *>(saltByteArray.constData());

    // prepare var to store result
    uint32_t outputBytes = 32; // keylen. 32 for sha256
    // 2*outputBytes+1 is 2 hex bytes per binary byte, and one character at the end for the string-terminating \0
    char hexResult[2 * outputBytes + 1];
    memset(hexResult, 0, sizeof(hexResult));

    // generating
    PBKDF2_HMAC_SHA_256_string(password, salt, iterations, outputBytes, hexResult);

    return QByteArray::fromHex(hexResult);
}

void CryptoService::PBKDF2_HMAC_SHA_256_string(const char* pass, const unsigned char* salt, int32_t iterations, uint32_t outputBytes, char* hexResult)
{
    unsigned int i;
    unsigned char digest[outputBytes];
    PKCS5_PBKDF2_HMAC(pass, strlen(pass), salt, strlen((char *)salt), iterations, EVP_sha256(), outputBytes, digest);
    for (i = 0; i < sizeof(digest); i++){
        sprintf(hexResult + (i * 2), "%02x", 255 & digest[i]);
    }
}
