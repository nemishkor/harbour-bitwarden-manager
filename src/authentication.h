#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <QDebug>
#include <QByteArray>
#include <QObject>
#include <QString>

#include "kdftype.h"

class Authentication : public QObject
{
    Q_OBJECT
public:
    explicit Authentication(QObject *parent = nullptr);
    ~Authentication();

    void clear();

    QString getEmail() const;
    void setEmail(const QString &value);

    KdfType getKdfType() const;
    void setKdfType(const KdfType &value);

    int getKdfIterations() const;
    void setKdfIterations(int value);

    QString getPassword() const;
    void setPassword(const QString &value);

    QString getHashedPassword() const;
    void setHashedPassword(const QString &value);

    QByteArray getKey() const;
    void setKey(const QByteArray &value);

private:
    QString email;
    KdfType kdfType;
    int kdfIterations;
    QString password;
    QString hashedPassword;
    QByteArray key;

signals:

};

#endif // AUTHENTICATION_H
