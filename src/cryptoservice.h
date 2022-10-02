#ifndef CRYPTO_H
#define CRYPTO_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QException>
#include <QDebug>
#include <QMessageAuthenticationCode>
#include <QSettings>

#include <tgmath.h>

#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/sha.h>
// crypto.h used for the version
#include <openssl/crypto.h>
#include <openssl/rand.h>

#include "string.h"
#include "cipherstring.h"
#include "kdftype.h"
//#include "sha.h"
#include "enums.h"
#include "symmetriccryptokey.h"

class CryptoService : public QObject
{
    Q_OBJECT

public:
    explicit CryptoService(QSettings *settings);
    QByteArray makeKey(QString password, QString email, KdfType kdf, int kdfIterations);
    QString hashPassword(QByteArray key, QString password);
    void setKey(SymmetricCryptoKey value);

    void setHashedPassword(const QString &value);

    SymmetricCryptoKey getEncKey();
    void setEncKey(const QString &encryptedValue);

    QString getPrivateKey() const;
    void setPrivateKey(const QString &value);

    // required to unlock process
    bool hasKey() const;

    // required to unlock process
    QString getHashedPassword() const;

    void clearKeys();
    void clearKey();
//    void clearOrgKeys();
    void clearKeyPair();
    void clearEncKey();

    QString decryptToUtf8(const CipherString &encrypted);
    QString encrypt(QString decrypted);

private:
    const int ivLength = 16;
    QSettings *settings;
    // Master Key
    // do not save key in QSettings. It is insecure
    // key created using master password and email
    SymmetricCryptoKey key;
    // Generated Symmetric Key
    // do not save encKey in QSettings. It is insecure
    // recieved on sync action
    SymmetricCryptoKey encKey;
    QString privateKey;

    // allows to unlock vault without internet using master passsword.
    // App does not save master password nor in RAM nor in QSettings
    QString hashedPassword;

    QByteArray pbkdf2(QByteArray password, QString salt, int iterations);
    void PBKDF2_HMAC_SHA_256_string(const char* pass, const unsigned char* salt, int32_t iterations, uint32_t outputBytes, char* hexResult);

    SymmetricCryptoKey getKeyForEncryption(const SymmetricCryptoKey *key = nullptr);
    SymmetricCryptoKey stretchKey(SymmetricCryptoKey key) const;
    const QByteArray hkdfExpandSHA256(QByteArray prk, QString info, int outputByteSize) const;
    QString aesDecryptToUtf8(Enums::EncryptionType encType, const QString data, const QString iv, const QString mac);
    QByteArray decryptToBytes(CipherString encryptedEncKey, SymmetricCryptoKey key) const;
    QByteArray QAesDecrypt(QByteArray data, QByteArray iv, QByteArray encKey) const;
    int aesDecrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
                unsigned char *iv, unsigned char *plaintext) const;
    QByteArray aesDecryptToBytes(Enums::EncryptionType encryptionType, QByteArray &data, QByteArray &iv, QByteArray &mac, SymmetricCryptoKey key) const;

    QByteArray QAesEncrypt(QByteArray plainText, QByteArray iv, QByteArray key);
    int aesEncrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext);
    QByteArray randomBytes(int length);

signals:
    void keyChanged();

};

#endif // CRYPTO_H
