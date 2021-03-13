#ifndef CRYPTO_H
#define CRYPTO_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QException>
#include <QDebug>
#include <QSettings>

#include <openssl/evp.h>
#include <openssl/sha.h>
// crypto.h used for the version
#include <openssl/crypto.h>

#include "kdftype.h"

class CryptoService : public QObject
{
    Q_OBJECT

public:
    explicit CryptoService(QSettings *settings);
    QByteArray makeKey(QString password, QString email, KdfType kdf, int kdfIterations);
    QString hashPassword(QByteArray key, QString password);
    void setKey(const QString &value);

    void setHashedPassword(const QString &value);

    QString getEncKey() const;
    void setEncKey(const QString &value);

    QString getPrivateKey() const;
    void setPrivateKey(const QString &value);

    // required to unlock process
    QString getKey() const;

    // required to unlock process
    QString getHashedPassword() const;

    void clearKey();
//    void clearOrgKeys();
    void clearKeyPair();
    void clearEncKey();

private:
    QSettings *settings;
    // do not save key in QSettings. It is insecure
    QString key; // Master Key
    QString encKey; // Generated Symmetric Key
    QString privateKey;

    // allows to unlock vault without internet using master passsword.
    // App does not save master password nor in RAM nor in QSettings
    QString hashedPassword;

    QByteArray pbkdf2(QByteArray password, QString salt, int iterations);
    void PBKDF2_HMAC_SHA_256_string(const char* pass, const unsigned char* salt, int32_t iterations, uint32_t outputBytes, char* hexResult);

signals:
    void keyChanged();

};

#endif // CRYPTO_H
