#include "cryptoservice.h"

CryptoService::CryptoService(QSettings *settings):
    QObject(nullptr),
    settings(settings)
{
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

void CryptoService::setKey(SymmetricCryptoKey value)
{
    key = value;
    qDebug() << "key was installed. Unlock state";
    emit keyChanged();
}

void CryptoService::setHashedPassword(const QString &value)
{
    hashedPassword = value;
    settings->setValue("hashedPassword", hashedPassword);
    settings->sync();
}

SymmetricCryptoKey CryptoService::getEncKey()
{
    if(!encKey.getKey().isNull()){
        return encKey;
    }

    if(key.getKey().isNull()){
        qWarning() << "key is null!";
        throw new QException;
    }

    QByteArray decryptedEncKey;
    qDebug() << "load enc key from QSettings";
    CipherString encKeyCipher = CipherString(settings->value("encKey").toString());
    if (encKeyCipher.getEncryptionType() == CipherString::EncryptionType::AesCbc256_B64) {
//        decryptedEncKey = decryptToBytes(encryptedEncKey); // ???
        qWarning() << "Does it heppens?";
        throw new QException();
    } else if (encKeyCipher.getEncryptionType() == CipherString::EncryptionType::AesCbc256_HmacSha256_B64) {
        SymmetricCryptoKey stratchedKey = stretchKey(this->key);
        encKey.fillWithKey(decryptToBytes(encKeyCipher, stratchedKey));
    } else {
        throw new QException();
    }

    return encKey;
}

void CryptoService::setEncKey(const QString &encryptedValue)
{
    encKey.clear();
    settings->setValue("encKey", encryptedValue);
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

bool CryptoService::hasKey() const
{
    return !key.getKey().isNull();
}

QString CryptoService::getHashedPassword() const
{
    return hashedPassword;
}

void CryptoService::clearKeys()
{
    qDebug() << "clear keys";
    clearKey();
//    this.clearKeyHash(),
//    this.clearOrgKeys(),
    clearEncKey();
    clearKeyPair();
//    this.clearPinProtectedKey(),

    hashedPassword.clear();
    settings->remove("hashedPassword");
    settings->sync();
}

void CryptoService::clearKey()
{
    key.clear();
    emit keyChanged();
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

QString CryptoService::decryptToUtf8(const CipherString &encrypted)
{
    return aesDecryptToUtf8(encrypted.getEncryptionType(), encrypted.getData(), encrypted.getIv(), encrypted.getMac());
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

SymmetricCryptoKey CryptoService::getKeyForEncryption()
{
    // TODO
    return getEncKey();
}

SymmetricCryptoKey CryptoService::stretchKey(SymmetricCryptoKey key) const
{
    QByteArray newKey;
    newKey.append(hkdfExpandSHA256(key.getKey(), "enc", 32));
    newKey.append(hkdfExpandSHA256(key.getKey(), "mac", 32));
    return SymmetricCryptoKey(newKey);
}

const QByteArray CryptoService::hkdfExpandSHA256(QByteArray prk, QString info, int outputByteSize) const
{
    int hashLen = 32;
    if(outputByteSize > 255 * hashLen){
        qWarning() << "outputByteSize is too large.";
        throw new QException();
    }
    if(prk.length() < hashLen){
        qWarning() << "prk is too small.";
        throw new QException();
    }
    int running0kmLength = 0;
    QByteArray previousT;
    int n = ceil(outputByteSize / hashLen);
    QByteArray okm;
    int i;
    QByteArray t;
    QMessageAuthenticationCode code(QCryptographicHash::Sha256);
    for(i = 0; i < n; i++){
        t.append(previousT);
        t.append(info);
        t.append(QChar(i + 1));
        code.setKey(prk);
        code.addData(t);
        previousT = QByteArray::fromBase64(code.result().toBase64());
        okm.append(previousT);
        running0kmLength += previousT.length();
        if(running0kmLength >= outputByteSize){
            break;
        }
        code.reset();
        t.clear();
    }
    return okm.mid(0, outputByteSize);
}

DecryptParametersArrayBuffer CryptoService::aesDecryptFastParameters(const QString data, const QString iv, const QString mac, SymmetricCryptoKey key) const
{
    DecryptParametersArrayBuffer p;

    p.encKey.append(key.getEncKey());
    p.data.append(QByteArray::fromBase64(data.toLatin1()));
    p.iv.append(QByteArray::fromBase64(iv.toLatin1()));
    for(int i = 0; i < p.iv.length(); i++) {
        p.macData.append(p.iv.at(i));
    }
    for(int i = 0; i < p.data.length(); i++) {
        p.macData.append(p.data.at(i));
    }

    if(!key.getMacKey().isNull()){
        p.macKey.append(key.getMacKey());
    }
    if(!mac.isNull()){
        qDebug() << "mac is not null";
        p.mac.append(QByteArray::fromBase64(mac.toLatin1()));
    }

    return p;
}

QString CryptoService::aesDecryptToUtf8(CipherString::EncryptionType encType, const QString data, const QString iv, const QString mac)
{
    SymmetricCryptoKey keyForEnc = getKeyForEncryption();
//    const theKey = this.resolveLegacyKey(encType, keyForEnc);

    if(!keyForEnc.getMacKey().isNull() && mac.isNull()){
        qWarning() << "mac required";
//        qDebug() << "data" << data;
//        qDebug() << "iv" << iv;
//        qDebug() << "mac" << mac;
        throw new QException();
    }

    if(keyForEnc.getEncType() != encType){
        qWarning() << "encType unavailable";
        throw new QException();
    }

    DecryptParametersArrayBuffer fastParams = aesDecryptFastParameters(data, iv, mac, keyForEnc);
    if(!fastParams.macKey.isEmpty() && !fastParams.mac.isEmpty()){
        QByteArray computedMac = QMessageAuthenticationCode::hash(fastParams.macData, fastParams.macKey, QCryptographicHash::Sha256);
        if(computedMac != fastParams.mac){
            qWarning() << "mac failed.";
            return QString();
        }
    }

    return QString(QAesDecrypt(fastParams.data, fastParams.iv, fastParams.encKey));
}

QByteArray CryptoService::decryptToBytes(CipherString encryptedEncKey, SymmetricCryptoKey key) const
{
    QByteArray iv;
    iv.append(encryptedEncKey.getIv());
    iv = QByteArray::fromBase64(iv);
    QByteArray data;
    data.append(encryptedEncKey.getData());
    data = QByteArray::fromBase64(data);
    QByteArray mac;
    if(!encryptedEncKey.getMac().isNull()){
        mac.append(encryptedEncKey.getMac());
        mac = QByteArray::fromBase64(mac);
    }

    return aesDecryptToBytes(encryptedEncKey.getEncryptionType(), data, iv, mac, key);
}

QByteArray CryptoService::aesDecryptToBytes(CipherString::EncryptionType encryptionType, QByteArray &data, QByteArray &iv, QByteArray &mac, SymmetricCryptoKey key) const
{
    //    const theKey = this.resolveLegacyKey(encType, keyForEnc);

    if(!key.getMacKey().isNull() && mac.isNull()){
        qWarning() << "key has mac key but mac is empty";
        throw new QException();
    }

    if(key.getEncType() != encryptionType){
        qWarning() << "encription types are not equal";
        throw new QException();
    }

    if(!key.getMacKey().isNull() && !mac.isNull()){
        qDebug() << "check mac";
        QByteArray macData;
        macData.append(iv);
        macData.append(data);
        QByteArray computedMac = QMessageAuthenticationCode::hash(macData, key.getMacKey(), QCryptographicHash::Sha256);
        if(computedMac != mac){
            qWarning() << "mac failed";
            throw new QException();
        }
    }

    return QAesDecrypt(data, iv, key.getEncKey());
}

QByteArray CryptoService::QAesDecrypt(QByteArray dataByteArray, QByteArray ivByteArray, QByteArray keyByteArray) const
{
    unsigned char *key = reinterpret_cast<unsigned char *>(keyByteArray.data());
    unsigned char *iv = reinterpret_cast<unsigned char *>(ivByteArray.data());
    unsigned char *data = reinterpret_cast<unsigned char *>(dataByteArray.data());
    int dataLength = dataByteArray.size();
    unsigned char *plainText = (unsigned char*) calloc(sizeof(char), dataLength+EVP_CIPHER_block_size(EVP_aes_256_cbc()));
//    unsigned char *plainText = (unsigned char*) malloc(120 + AES_BLOCK_SIZE);

    qDebug() << "aesDecrypt";
    int decryptedtext_len = aesDecrypt(data, dataLength, key, iv, plainText);


    QByteArray output = QByteArray(reinterpret_cast<char*> (plainText), decryptedtext_len);
    free(plainText);

    return output;
}

int CryptoService::aesDecrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext) const
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int plaintext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new())){
        qWarning() << "can not create cipher context";
        throw new QException();
    }

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)){
        EVP_CIPHER_CTX_free(ctx);
        qCritical() << "EVP_DecryptInit_ex() -- " << ERR_error_string(ERR_get_error(), NULL);
        throw new QException();
    }

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)){
        qCritical() << "EVP_DecryptUpdate() -- " << ERR_error_string(ERR_get_error(), NULL);
        EVP_CIPHER_CTX_free(ctx);
        throw new QException();
    }

    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)){
        EVP_CIPHER_CTX_free(ctx);
        qCritical() << "EVP_DecryptInit_ex() -- " << ERR_error_string(ERR_get_error(), NULL);
        throw new QException();
    }

    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}
