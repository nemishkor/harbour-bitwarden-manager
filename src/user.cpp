#include "user.h"

User::User(QSettings *settings) : QObject(nullptr), settings(settings)
{
    if(settings->contains("userId"))
        userId = settings->value("userId").toString();
    if(settings->contains("email"))
        email = settings->value("email").toString();
    if(settings->contains("kdf"))
        kdf = static_cast<KdfType>(settings->value("kdf").toInt());
    if(settings->contains("kdfIterations"))
        kdfIterations = settings->value("kdfIterations").toInt();
    if(settings->contains("stamp"))
        stamp = settings->value("securityStamp").toString();
}

void User::setInformation(QString userId, QString email, KdfType kdf, int kdfIterations)
{
    qDebug() << "Authorized user with id" << userId << "and email" << email;
    setUserId(userId);
    setEmail(email);
    this->kdf = kdf;
    settings->setValue("kdf", (int)kdf);
    this->kdfIterations = kdfIterations;
    settings->setValue("kdfIterations", kdfIterations);
    settings->sync();
}

QString User::getUserId() const
{
    return userId;
}

QString User::getEmail()
{
    return email;
}

bool User::existsInformation()
{
    return userId != "" && email != "";
}

QString User::getStamp() const
{
    return stamp;
}

void User::setStamp(const QString &value)
{
    stamp = value;
    settings->setValue("securityStamp", stamp);
    settings->sync();
}

QString User::getName() const
{
    return name;
}

void User::setName(const QString &value)
{
    if(name != value){
        name = value;
        emit nameChanged();
    }
}

bool User::getPremium() const
{
    return premium;
}

void User::setPremium(bool value)
{
    if(premium != value){
        premium = value;
        emit premiumChanged();
    }
}

void User::setEmail(const QString &value)
{
    if(email != value){
        email = value;
        settings->setValue("email", email);
        emit emailChanged();
    }
}

void User::setUserId(const QString &value)
{
    if(userId != value){
        userId = value;
        settings->setValue("userId", userId);
    }
}
