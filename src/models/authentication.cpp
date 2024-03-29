#include "authentication.h"

Authentication::Authentication(QObject *parent) : QObject(parent)
{
}

void Authentication::clear()
{
    email.clear();
    password.clear();
    apiKey.clear();
    hashedPassword.clear();
    key.clear();
}

QString Authentication::getEmail() const
{
    return email;
}

void Authentication::setEmail(const QString &value)
{
    email = value;
}

QString Authentication::getPassword() const
{
    return password;
}

void Authentication::setPassword(const QString &value)
{
    password = value;
}

QString Authentication::getHashedPassword() const
{
    return hashedPassword;
}

void Authentication::setHashedPassword(const QString &value)
{
    hashedPassword = value;
    password.clear();
}

QByteArray Authentication::getKey() const
{
    return key;
}

void Authentication::setKey(const QByteArray &value)
{
    key = value;
}

const QString &Authentication::getApiKey() const
{
    return apiKey;
}

void Authentication::setApiKey(const QString &newApiKey)
{
    apiKey = newApiKey;
}

KdfType Authentication::getKdfType() const
{
    return kdfType;
}

void Authentication::setKdfType(const KdfType &value)
{
    kdfType = value;
}

int Authentication::getKdfIterations() const
{
    return kdfIterations;
}

void Authentication::setKdfIterations(int value)
{
    kdfIterations = value;
}
