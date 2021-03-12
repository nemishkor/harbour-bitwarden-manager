#ifndef AUTH_H
#define AUTH_H

#include <QObject>
#include <QDebug>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QList>
#include <QUrlQuery>

#include "kdftype.h"
#include "api.h"
#include "cryptoservice.h"
#include "appidservice.h"
#include "devicetype.h"
#include "tokenservice.h"
#include "user.h"

class Auth : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString email READ getEmail NOTIFY emailChanged);
    Q_PROPERTY(bool loginProcessing READ isLoginProcessing NOTIFY loginProcessingChanged);
    Q_PROPERTY(QString loginMessage READ getLoginMessage NOTIFY loginMessageChanged);
    Q_PROPERTY(QString loginMessageType READ getLoginMessageType NOTIFY loginMessageTypeChanged);
public:
    Auth(AppIdService *appIdService, TokenService *tokenService, Api *api, CryptoService *crypto, User *user);
    Q_INVOKABLE void login(QString password);
    QString getEmail();
    bool isLoginProcessing();
    QString getLoginMessage();
    QString getLoginMessageType();

signals:
    void emailChanged();
    void loginProcessingChanged();
    void loginMessageChanged();
    void loginMessageTypeChanged();

private:
    AppIdService *appIdService;
    TokenService *tokenService;
    Api *api;
    CryptoService *crypto;
    User *user;

    bool loginProcessing = false;
    QString loginMessage;
    QString loginMessageType;
    void setLoginMessage(QString message, QString type = "info");

    QString email;
    QString password;
    QString hashedPassword;

    QNetworkReply *preloginReply;
    QNetworkReply *identityReply;
    void prelogin();
    void makePreloginKey();
    QByteArray makeIdentityTokenRequestBody();
    void identityReplyFinished();

};

#endif // AUTH_H
