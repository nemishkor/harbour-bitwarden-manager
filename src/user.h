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
    Q_PROPERTY(bool authenticated READ isAuthenticated NOTIFY authenticatedChanged)
    Q_PROPERTY(QString email READ getEmail NOTIFY emailChanged)
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
    Q_PROPERTY(QString premium READ getPremium NOTIFY premiumChanged)
public:
    User(QSettings *settings);

    // on successfully login
    void setInformation(QString userId, QString email, KdfType kdf, int kdfIterations);

    QString getUserId() const;
    void setUserId(const QString &value);

    QString getEmail();
    void setEmail(const QString &value);

    QString getStamp() const;
    void setStamp(const QString &value);

    QString getName() const;
    void setName(const QString &value);

    bool getPremium() const;
    void setPremium(bool value);

    // required to unlock vault
    KdfType getKdf() const;

    // required to unlock vault
    int getKdfIterations() const;

    void clear();

    bool isAuthenticated() const;

private:
    QSettings *settings;
    bool authenticated;
    QString userId;
    QString email;
    KdfType kdf;
    int kdfIterations;
    QString stamp;
    QString name;
    bool premium;

signals:
    void authenticatedChanged();
    void emailChanged();
    void nameChanged();
    void premiumChanged();

};

#endif // USER_H
