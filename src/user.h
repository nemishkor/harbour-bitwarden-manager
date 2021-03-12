#ifndef USER_H
#define USER_H

#include <QObject>
#include <QDebug>

#include "kdftype.h"

/**
 * Authorized user
 */
class User : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString email READ getEmail NOTIFY emailChanged)
public:
    explicit User(QObject *parent = nullptr);
    void setInformation(QString userId, QString email, KdfType kdf, int kdfIterations);
    QString getEmail();

private:
    QString userId;
    QString email;
    KdfType kdf;
    int kdfIterations;

signals:
    void emailChanged();

};

#endif // USER_H
