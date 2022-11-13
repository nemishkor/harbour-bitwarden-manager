#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <QDebug>
#include <QByteArray>
#include <QObject>
#include <QString>

#include "src/kdftype.h"

class Authentication : public QObject
{
    Q_OBJECT
public:
    explicit Authentication(QObject *parent = nullptr);

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

    const QString &getApiKey() const;
    void setApiKey(const QString &newApiKey);

private:
    QString email;
    KdfType kdfType;
    int kdfIterations;
    QString password;
    QString apiKey;
    QString hashedPassword;
    QByteArray key;

signals:

};

#endif // AUTHENTICATION_H
