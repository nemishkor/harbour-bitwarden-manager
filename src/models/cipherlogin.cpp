#include "cipherlogin.h"

CipherLogin::CipherLogin()
{
    password = nullptr;
    uri = nullptr;
    username = nullptr;
    totp = nullptr;
}

CipherLogin::~CipherLogin()
{
    delete password;
    delete uri;
    delete username;
    delete totp;
}

QString CipherLogin::getPasswordRevisionDate() const
{
    return passwordRevisionDate;
}

void CipherLogin::setPasswordRevisionDate(const QString &value)
{
    passwordRevisionDate = value;
}

CipherString* CipherLogin::getPassword() const
{
    return password;
}

void CipherLogin::fillPassword(const QString &value)
{
    if(password == nullptr){
        password = new CipherString();
    }
    password->fill(value);
}

CipherString* CipherLogin::getUri() const
{
    return uri;
}

void CipherLogin::fillUri(const QString &value)
{
    if(uri == nullptr){
        uri = new CipherString();
    }
    uri->fill(value);
}

CipherString* CipherLogin::getUsername() const
{
    return username;
}

void CipherLogin::fillUsername(const QString &value)
{
    if(username == nullptr){
        username = new CipherString();
    }
    username->fill(value);
}

CipherString* CipherLogin::getTotp() const
{
    return totp;
}

void CipherLogin::fillTotp(const QString &value)
{
    if(totp == nullptr){
        totp = new CipherString();
    }
    totp->fill(value);
}
