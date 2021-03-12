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
}

void User::setInformation(QString userId, QString email, KdfType kdf, int kdfIterations)
{
    qDebug() << "Authorized user with id" << userId << "and email" << email;
    this->userId = userId;
    this->email = email;
    this->kdf = kdf;
    this->kdfIterations = kdfIterations;
    emit emailChanged();
}

QString User::getEmail()
{
    return email;
}

bool User::existsInformation()
{
    return userId != "" && email != "";
}

void User::persistInformation()
{
    settings->setValue("userId", userId);
    settings->setValue("email", email);
    settings->setValue("kdf", (int)kdf);
    settings->setValue("kdfIterations", kdfIterations);
}
