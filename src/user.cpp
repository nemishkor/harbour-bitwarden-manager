#include "user.h"

User::User(QSettings *settings) : QObject(nullptr), settings(settings)
{
    if(settings->contains("userId")){
        userId = settings->value("userId").toString();
        email = settings->value("email").toString();
        kdf = static_cast<KdfType>(settings->value("kdf").toInt());
        kdfIterations = settings->value("kdfIterations").toInt();
        stamp = settings->value("securityStamp").toString();
        authenticated = true;
        emit authenticatedChanged();
    }
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
    authenticated = true;
    emit authenticatedChanged();
}

QString User::getUserId() const
{
    return userId;
}

QString User::getEmail()
{
    return email;
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

KdfType User::getKdf() const
{
    return kdf;
}

int User::getKdfIterations() const
{
    return kdfIterations;
}

void User::clear()
{
    qDebug() << "clear user's data";
    userId.clear();
    settings->remove("userId");
    email.clear();
    settings->remove("email");
    kdfIterations = 0;
    stamp.clear();
    settings->remove("securityStamp");
    name.clear();
    settings->sync();
    authenticated = false;
    emit authenticatedChanged();
}

bool User::isAuthenticated() const
{
    return authenticated;
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
