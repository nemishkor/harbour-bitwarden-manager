#ifndef VAULTMANAGER_H
#define VAULTMANAGER_H

#include <QByteArray>
#include <QObject>
#include <QNetworkReply>
#include <QString>

#include "api.h"
#include "cryptoservice.h"
#include "tokenservice.h"
#include "user.h"

class VaultManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isLocked READ isLocked NOTIFY isLockedChanged)
    Q_PROPERTY(bool unlocking READ getUnlocking NOTIFY unlockingChanged)
    Q_PROPERTY(QString unlockMessage READ getUnlockMessage NOTIFY unlockMessageChanged)

public:
    explicit VaultManager(CryptoService *cryptoService, User *user, Api *api, TokenService *tokenService);
    Q_INVOKABLE void unlock(QString password);
    Q_INVOKABLE void lock();
    bool isLocked();
    void keyChanged();

    bool getUnlocking() const;
    QString getUnlockMessage() const;

private:
    QByteArray key; // temporary required for password verification process to unlock vault
    QString masterPasswordHash; // temporary required for password verification process to unlock vault
    CryptoService *cryptoService;
    User *user;
    Api *api;
    TokenService *tokenService;
    QNetworkReply *refreshTokenReply;
    QNetworkReply *verifyPasswordReply;
    void refreshTokenReplyFinished();
    void verifyPasswordOnline();
    void verifyPasswordFinished();

    // for GUI
    bool unlocking;
    void setUnlocking(const bool &value);
    QString unlockMessage;
    void setUnlockMessage(const QString &value);

signals:
    void isLockedChanged();
    void unlockingChanged();
    void unlockMessageChanged();

};

#endif // VAULTMANAGER_H
