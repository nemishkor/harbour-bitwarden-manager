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
#include "symmetriccryptokey.h"
#include "models/responses/identitycaptcharesponse.h"

class Auth : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int loginStage READ getLoginStage NOTIFY loginStageChanged);
    Q_PROPERTY(QString loginMessage READ getLoginMessage NOTIFY loginMessageChanged);
    Q_PROPERTY(QString loginMessageType READ getLoginMessageType NOTIFY loginMessageTypeChanged);
    Q_PROPERTY(bool apiKeyRequired READ isApiKeyRequired NOTIFY isApiKeyRequiredChanged);
public:
    Auth(AppIdService *appIdService, TokenService *tokenService, Api *api, CryptoService *crypto, User *user);
    Q_INVOKABLE void reset();
    Q_INVOKABLE void login(QString email, QString password, QString apiKey);
    Q_INVOKABLE void logout();
    int getLoginStage();
    QString getLoginMessage();
    QString getLoginMessageType();

    bool isApiKeyRequired() const;
    void setIsApiKeyRequired(bool newIsApiKeyRequired);

signals:
    void loginStageChanged();
    void loginMessageChanged();
    void loginMessageTypeChanged();
    void isApiKeyRequiredChanged();

private:
    AppIdService *appIdService;
    TokenService *tokenService;
    Api *api;
    CryptoService *crypto;
    User *user;
    bool apiKeyRequired = false;

    /*
     * Login stages:
     * -1 = starting. GUI should be blocked
     * 0 = required email and password
     * 1 = GUI must wait. Receiving preLogin response. Password hashing, key encryption and tokens loading
     * 2 = tokens saved, successfully authenticated
     */

    // authenticate related things for GUI
    int loginStage = 0;
    void setLoginStage(int value);

    void preLogin();
    void getToken();

    QString loginMessage;
    QString loginMessageType;
    void setLoginMessage(QString message, QString type = "info");

    // data required for authentication process
    Authentication *authentication;

    QNetworkReply *preloginReply;
    QNetworkReply *authenticateReply;
    void preLoginReplyHandler();
    QByteArray makeIdentityTokenRequestBody();
    void postAuthenticate();

};

#endif // AUTH_H
