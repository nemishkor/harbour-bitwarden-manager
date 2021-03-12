#ifndef CRYPTO_H
#define CRYPTO_H

#include <QString>
#include <QByteArray>
#include <QException>
#include <QDebug>
#include <openssl/evp.h>
#include <openssl/sha.h>
// crypto.h used for the version
#include <openssl/crypto.h>

#include "kdftype.h"

class CryptoService
{
public:
    CryptoService();
    QByteArray makeKey(QString password, QString email, KdfType kdf, int kdfIterations);
    QString hashPassword(QByteArray key, QString password);
private:
    QByteArray pbkdf2(QByteArray password, QString salt, int iterations);
    void PBKDF2_HMAC_SHA_256_string(const char* pass, const unsigned char* salt, int32_t iterations, uint32_t outputBytes, char* hexResult);
};

#endif // CRYPTO_H
