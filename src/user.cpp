#include "user.h"

User::User(QSettings *settings, QObject* parent) : QObject(parent), settings(settings)
{
    setIsAuthenticated(false);
    if(settings->contains("userId") && settings->contains("email")){
        QString userId = settings->value("userId").toString();
        QString email = settings->value("email").toString();
        KdfType kdf = static_cast<KdfType>(settings->value("kdf").toInt());
        int kdfIterations = settings->value("kdfIterations").toInt();
        setInformation(userId, email, kdf, kdfIterations);
        stamp = settings->value("securityStamp").toString();
    }
}

void User::fillOnAutheticate(QString userId, QString email, KdfType kdf, int kdfIterations)
{
    setInformation(userId, email, kdf, kdfIterations);
    settings->sync();
}

void User::setInformation(QString userId, QString email, KdfType kdf, int kdfIterations)
{
    qDebug() << "Authorize user with id" << userId
             << "and email" << email
             << "and kdf" << QString::number((int)kdf)
             << "and kdfIterations" << QString::number(kdfIterations);
    if(userId.isEmpty()){
        qWarning() << "Unable to authorize user: userId is empty";
        return;
    }
    if(email.isEmpty()){
        qWarning() << "Unable to authorize user: email is empty";
        return;
    }
    setUserId(userId);
    setEmail(email);
    setKdf(kdf);
    setKdfIterations(kdfIterations);
    setIsAuthenticated(true);
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
    email.clear();
    kdfIterations = 0;
    stamp.clear();
    name.clear();
    settings->remove("userId");
    settings->remove("email");
    settings->remove("securityStamp");
    settings->remove("kdf");
    settings->remove("kdfIterations");
    settings->sync();
    setIsAuthenticated(false);
}

bool User::isAuthenticated() const
{
    return authenticated;
}

void User::setIsAuthenticated(bool newIsAuthenticated)
{
    if(authenticated != newIsAuthenticated){
        qDebug() << "Authorized status:" << (newIsAuthenticated ? "yes" : "no");
        authenticated = newIsAuthenticated;
        emit authenticatedChanged();
    }
}

void User::setKdf(KdfType newKdf)
{
    kdf = newKdf;
    settings->setValue("kdf", (int)newKdf);
}

void User::setKdfIterations(int newKdfIterations)
{
    kdfIterations = newKdfIterations;
    settings->setValue("kdfIterations", newKdfIterations);
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
