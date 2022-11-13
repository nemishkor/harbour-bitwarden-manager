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
#include "devicetype.h"
#include "user.h"
#include "symmetriccryptokey.h"
#include "src/services/appidservice.h"
#include "src/services/cryptoservice.h"
#include "src/services/syncservice.h"
#include "src/services/tokenservice.h"
#include "models/responses/identitycaptcharesponse.h"
#include "src/models/authentication.h"
#include "src/models/tasklistitem.h"
#include "src/models/taskslistmodel.h"

class Auth : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int loginStage READ getLoginStage NOTIFY loginStageChanged);
    Q_PROPERTY(bool apiKeyRequired READ isApiKeyRequired NOTIFY isApiKeyRequiredChanged);
public:
    Auth(AppIdService *appIdService,
         TokenService *tokenService,
         Api *api,
         CryptoService *crypto,
         User *user,
         SyncService *syncService,
         TasksListModel *tasksListModel,
         QObject* parent = nullptr);
    Q_INVOKABLE void reset();
    Q_INVOKABLE void login(QString email, QString password, QString apiKey);
    Q_INVOKABLE void logout();
    int getLoginStage();

    bool isApiKeyRequired() const;
    void setIsApiKeyRequired(bool newIsApiKeyRequired);

signals:
    void loginStageChanged();
    void isApiKeyRequiredChanged();

private:
    AppIdService *appIdService;
    TokenService *tokenService;
    Api *api;
    CryptoService *crypto;
    User *user;
    SyncService *syncService;
    TasksListModel *tasksListModel;
    TaskListItem *loginTask;
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

    // data required for authentication process
    Authentication *authentication;

    QNetworkReply *preloginReply;
    QNetworkReply *authenticateReply;
    void preLoginReplyHandler();
    QByteArray makeIdentityTokenRequestBody();
    void postAuthenticate();

private slots:
    void loginTaskWasFinished();

};

#endif // AUTH_H
