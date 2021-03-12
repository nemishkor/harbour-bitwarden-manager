#include "user.h"

User::User(QObject *parent) : QObject(parent)
{

}

void User::setInformation(QString userId, QString email, KdfType kdf, int kdfIterations)
{
    qDebug() << "Authorized user with id" << userId << "and email" << email;
    this->userId = userId;
    this->email = email;
    this->kdf = kdf;
    this->kdfIterations = kdfIterations;
}
