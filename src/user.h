#ifndef USER_H
#define USER_H

#include <QObject>
#include <QDebug>
#include <QSettings>

#include "kdftype.h"

/**
 * Authorized user
 */
class User : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString email READ getEmail NOTIFY emailChanged)
public:
    User(QSettings *settings);
    void setInformation(QString userId, QString email, KdfType kdf, int kdfIterations);
    QString getEmail();
    bool existsInformation();

private:
    QSettings *settings;
    QString userId;
    QString email;
    KdfType kdf;
    int kdfIterations;
    void persistInformation();

signals:
    void emailChanged();

};

#endif // USER_H
