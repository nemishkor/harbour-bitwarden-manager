#ifndef CIPHERLOGIN_H
#define CIPHERLOGIN_H

#include <QDebug>
#include <QString>

#include "cipherstring.h"

class CipherLogin
{
public:
    CipherLogin();
    CipherLogin(const CipherLogin& login) :
        password(login.password),
        passwordRevisionDate(login.passwordRevisionDate),
        uri(login.uri),
        username(login.username),
        totp(login.totp)
    {
        Q_UNUSED(login);
        qDebug() << "copy cipherLogin";
    }

    QString getPasswordRevisionDate() const;
    void setPasswordRevisionDate(const QString &value);

    CipherString getPassword() const;
    void fillPassword(const QString &password);

    CipherString getUri() const;
    void fillUri(const QString &value);

    CipherString getUsername() const;
    void fillUsername(const QString &value);

    CipherString getTotp() const;
    void fillTotp(const QString &value);

private:
    CipherString password;
    QString passwordRevisionDate;
    CipherString uri;
    CipherString username;
    CipherString totp;

};

#endif // CIPHERLOGIN_H
