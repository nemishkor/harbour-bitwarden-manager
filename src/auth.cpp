#include "auth.h"

Auth::Auth(AppIdService *appIdService, TokenService *tokenService, Api *api, CryptoService *crypto, User *user) :
    appIdService(appIdService),
    tokenService(tokenService),
    api(api),
    crypto(crypto),
    user(user)
{
    authentication = new Authentication();
    if(tokenService->exists()){
        qDebug() << "tokens is exist";
    }
    if(user->existsInformation()){
        qDebug() << "user's information is exists";
    }
    if(tokenService->exists() && user->existsInformation()){
        setLoginStage(4);
    }
}

int Auth::getLoginStage()
{
    return loginStage;
}

QString Auth::getLoginMessage()
{
    return loginMessage;
}

QString Auth::getLoginMessageType()
{
    return loginMessageType;
}

void Auth::setLoginStage(int value)
{
    if(value != loginStage){
        loginStage = value;
        emit loginStageChanged();
    }
}

void Auth::setLoginMessage(QString message, QString type)
{
    if(this->loginMessage != message){
        this->loginMessage = message;
        emit loginMessageChanged();
    }
    if(this->loginMessageType != type){
        this->loginMessageType = type;
        emit loginMessageTypeChanged();
    }
}

void Auth::preLogin(QString email)
{
    setLoginStage(1);
    setLoginMessage("Checking encoding settings");
    authentication->setEmail(email);
    QJsonObject jsonBody{ {"email", authentication->getEmail()} };
    // send request to load key derivation function parameters
    preloginReply = api->postPrelogin(QUrl(api->getApiUrl() + "/accounts/prelogin"), QJsonDocument(jsonBody).toJson(QJsonDocument::Compact));
    connect(preloginReply, &QNetworkReply::finished, this, &Auth::saveKDFParameters);
}

void Auth::saveKDFParameters()
{
    if(preloginReply->error() != QNetworkReply::NoError){
        setLoginStage(0);
        setLoginMessage(preloginReply->errorString(), "error");
        return;
    }

    QJsonDocument json = QJsonDocument::fromJson(preloginReply->readAll());
    if(!json.isObject()){
        setLoginMessage("Invalid prelogin API response #1", "error");
        return;
    }
    QJsonObject root = json.object();
    if(!root.contains("Kdf")){
        setLoginMessage("Invalid prelogin API response #2", "error");
        return;
    }
    if(!root.contains("KdfIterations")){
        setLoginMessage("Invalid prelogin API response #3", "error");
        return;
    }
    authentication->setKdfType(static_cast<KdfType>(root["Kdf"].toInt()));
    authentication->setKdfIterations(root["KdfIterations"].toInt());

    setLoginStage(2);
    setLoginMessage("");
}

void Auth::login(QString password)
{
    setLoginStage(3);
    setLoginMessage("Hashing");
    authentication->setKey(crypto->makeKey(password, authentication->getEmail(), authentication->getKdfType(), authentication->getKdfIterations()));
    authentication->setHashedPassword(crypto->hashPassword(authentication->getKey(), password));
    setLoginMessage("Logging in");
    authenticateReply = api->postIdentityToken(api->getIdentityUrl() + "/connect/token", makeIdentityTokenRequestBody());
    connect(authenticateReply, &QNetworkReply::finished, this, &Auth::postAuthenticate);
}

QByteArray Auth::makeIdentityTokenRequestBody()
{
    QUrlQuery query;
    query.addQueryItem("scope", "api offline_access");
    query.addQueryItem("client_id", "cli");
    query.addQueryItem("grant_type", "password"); // auth via password
    query.addQueryItem("username", QUrl::toPercentEncoding(authentication->getEmail()));
    query.addQueryItem("password", QUrl::toPercentEncoding(authentication->getHashedPassword(), "%"));
    query.addQueryItem("deviceType", QString::number(static_cast<int>(DeviceType::LinuxDesktop)));
    query.addQueryItem("deviceIdentifier", appIdService->getAppId());
    query.addQueryItem("deviceName", "linux"); // TODO: try to change to real device name

    QByteArray body;
    body.append(query.toString(QUrl::FullyEncoded));

    return body;
}

void Auth::postAuthenticate()
{
    if(authenticateReply->error() != QNetworkReply::NoError){
        setLoginStage(0);
        setLoginMessage(authenticateReply->errorString(), "error");
        return;
    }

    QJsonDocument json = QJsonDocument::fromJson(authenticateReply->readAll());
    qDebug() << "json response:" << json.toJson(QJsonDocument::Compact);

    if(!json.isObject()){
        setLoginMessage("Invalid API response", "error");
        return;
    }

    QJsonObject root = json.object();
    if(root.contains("ErrorModel")){
        setLoginMessage(root["ErrorModel"].toObject()["Message"].toString(), "error");
        return;
    }
    if(root.contains("error_description")) {
        setLoginMessage(root["error_description"].toString(), "error");
        return;
    }
    if(root.contains("error")) {
        setLoginMessage(root["error"].toString(), "error");
        return;
    }

    if(!root.contains("access_token")){
        setLoginMessage("Invalid API response #2", "error");
        return;
    }
    if(!root.contains("refresh_token")){
        setLoginMessage("Invalid API response #3", "error");
        return;
    }

    tokenService->setTokens(root["access_token"].toString(), root["refresh_token"].toString());
    user->setInformation(tokenService->getUserIdFromToken(), tokenService->getEmailFromToken(), static_cast<KdfType>(root["Kdf"].toInt()), root["KdfIterations"].toInt());

    setLoginStage(4);
    setLoginMessage("");
}

void Auth::abortAuthentication()
{
    authentication->clear();
    loginStage = 0;
    if(preloginReply->isRunning()){
        preloginReply->abort();
    }
    if(authenticateReply->isRunning()){
        authenticateReply->abort();
    }
}
