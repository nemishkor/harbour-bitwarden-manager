#include "cipherlogin.h"

CipherLogin::CipherLogin()
{

}

QString CipherLogin::getPasswordRevisionDate() const
{
    return passwordRevisionDate;
}

void CipherLogin::setPasswordRevisionDate(const QString &value)
{
    passwordRevisionDate = value;
}

CipherString CipherLogin::getPassword() const
{
    return password;
}

void CipherLogin::fillPassword(const QString &value)
{
    password.fill(value);
}

CipherString CipherLogin::getUri() const
{
    return uri;
}

void CipherLogin::fillUri(const QString &value)
{
    uri.fill(value);
}

CipherString CipherLogin::getUsername() const
{
    return username;
}

void CipherLogin::fillUsername(const QString &value)
{
    username.fill(value);
}
