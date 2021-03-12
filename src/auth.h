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
#include "authentication.h"
#include "cryptoservice.h"
#include "appidservice.h"
#include "devicetype.h"
#include "tokenservice.h"
#include "user.h"

class Auth : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int loginStage READ getLoginStage NOTIFY loginStageChanged);
    Q_PROPERTY(QString loginMessage READ getLoginMessage NOTIFY loginMessageChanged);
    Q_PROPERTY(QString loginMessageType READ getLoginMessageType NOTIFY loginMessageTypeChanged);
public:
    Auth(AppIdService *appIdService, TokenService *tokenService, Api *api, CryptoService *crypto, User *user);
    Q_INVOKABLE void reset();
    Q_INVOKABLE void preLogin(QString email);
    Q_INVOKABLE void login(QString password);
    int getLoginStage();
    QString getLoginMessage();
    QString getLoginMessageType();

signals:
    void loginStageChanged();
    void loginMessageChanged();
    void loginMessageTypeChanged();

private:
    AppIdService *appIdService;
    TokenService *tokenService;
    Api *api;
    CryptoService *crypto;
    User *user;

    /*
     * Login stages:
     * -1 = starting. GUI should be blocked
     * 0 = required email to run preLogin
     * 1 = GUI must wait. Receiving preLogin response
     * 2 = required password to hash password and encrypt key
     * 3 = GUI must wait. Password hashing, key encryption and tokens loading
     * 4 = tokens saved, successfully authenticated
     */

    // authenticate related things for GUI
    int loginStage = 0;
    void setLoginStage(int value);
    QString loginMessage;
    QString loginMessageType;
    void setLoginMessage(QString message, QString type = "info");

    // data required for authentication process
    Authentication *authentication;

    QNetworkReply *preloginReply;
    QNetworkReply *authenticateReply;
    void saveKDFParameters();
    QByteArray makeIdentityTokenRequestBody();
    void postAuthenticate();

};

#endif // AUTH_H
