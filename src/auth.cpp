#include "auth.h"

Auth::Auth(AppIdService *appIdService, TokenService *tokenService, Api *api, CryptoService *crypto, User *user) :
    appIdService(appIdService),
    tokenService(tokenService),
    api(api),
    crypto(crypto),
    user(user)
{
    authentication = new Authentication();
    preloginReply = nullptr;
    authenticateReply = nullptr;
    if(tokenService->exists()){
        qDebug() << "tokens are exist";
        if(user->isAuthenticated()){
            qDebug() << "user is authenticated";
            setLoginStage(4);
        }
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

bool Auth::isApiKeyRequired() const
{
    return apiKeyRequired;
}

void Auth::setIsApiKeyRequired(bool newIsApiKeyRequired)
{
    if(apiKeyRequired != newIsApiKeyRequired){
        apiKeyRequired = newIsApiKeyRequired;
        emit isApiKeyRequiredChanged();
    }
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

void Auth::reset()
{
    authentication->clear();
    setLoginStage(0);
    setIsApiKeyRequired(false);
    if(preloginReply && preloginReply->isRunning()){
        preloginReply->abort();
    }
    if(authenticateReply && authenticateReply->isRunning()){
        authenticateReply->abort();
    }
}

void Auth::login(QString email, QString password, QString apiKey)
{
    authentication->clear();
    authentication->setEmail(email);
    authentication->setPassword(password);
    authentication->setApiKey(apiKey);
    setLoginStage(1);
    preLogin();
}

void Auth::preLogin()
{
    setLoginMessage("Checking encoding settings");
    // send request to load key derivation function parameters
    preloginReply = api->postPrelogin(authentication->getEmail());
    connect(preloginReply, &QNetworkReply::finished, this, &Auth::preLoginReplyHandler);
}

void Auth::preLoginReplyHandler()
{
    if(preloginReply->error() != QNetworkReply::NoError){
        setLoginMessage("[" + QString::number(preloginReply->error()) + "]" + preloginReply->errorString(), "error");
        reset();
        return;
    }

    QByteArray replyBody = preloginReply->readAll();
    qDebug() << "Prelogin reply body:\n" << replyBody;

    QJsonDocument json = QJsonDocument::fromJson(replyBody);
    if(!json.isObject()){
        setLoginMessage("Invalid prelogin API response: root item is not a object", "error");
        reset();
        return;
    }
    QJsonObject root = json.object();
    if(!root.contains("kdf")){
        setLoginMessage("Invalid prelogin API response: root object does not have \"kdf\" key", "error");
        reset();
        return;
    }
    if(!root.contains("kdfIterations")){
        setLoginMessage("Invalid prelogin API response: root object does not have \"kdfIterations\" key", "error");
        reset();
        return;
    }
    authentication->setKdfType(static_cast<KdfType>(root["kdf"].toInt()));
    authentication->setKdfIterations(root["kdfIterations"].toInt());
    getToken();
}

void Auth::getToken()
{
    setLoginMessage("Hashing");
    authentication->setKey(crypto->makeKey(authentication->getPassword(), authentication->getEmail(), authentication->getKdfType(), authentication->getKdfIterations()));
    authentication->setHashedPassword(crypto->hashPassword(authentication->getKey(), authentication->getPassword()));
    setLoginMessage("Logging in");
    authenticateReply = api->postIdentityToken(makeIdentityTokenRequestBody(), authentication->getEmail());
    connect(authenticateReply, &QNetworkReply::finished, this, &Auth::postAuthenticate);
}

void Auth::logout()
{
    reset();
//    this.syncService.setLastSync(new Date(0)),
    tokenService->clearTokens();
    crypto->clearKeys();
    user->clear();
//    this.settingsService.clear(userId),
//    this.cipherService.clear(userId),
//    this.folderService.clear(userId),
//    this.collectionService.clear(userId),
//    this.policyService.clear(userId),
//    this.passwordGenerationService.clear(),
    qDebug() << "logout finished";
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

    if(apiKeyRequired){
        query.addQueryItem("captchaResponse", authentication->getApiKey());
    }

    QByteArray body;
    body.append(query.toString(QUrl::FullyEncoded));

    return body;
}

void Auth::postAuthenticate()
{
    if(authenticateReply->error() != QNetworkReply::NoError && authenticateReply->error() != QNetworkReply::ProtocolInvalidOperationError){
        setLoginMessage("[" + QString::number(authenticateReply->error()) + "]" + authenticateReply->errorString(), "error");
        reset();
        return;
    }

    QByteArray replyBody = authenticateReply->readAll();
    qDebug() << "Authentication reply body:\n" << replyBody;

    QJsonDocument json = QJsonDocument::fromJson(replyBody);
    qDebug() << "json response:" << json.toJson(QJsonDocument::Compact);

    if(!json.isObject()){
        setLoginMessage("Invalid API response", "error");
        reset();
        return;
    }

    QJsonObject root = json.object();
    int httpCode = authenticateReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if(httpCode == 400 && root.contains("HCaptcha_SiteKey")){
        IdentityCaptchaResponse identityCaptchaResponse(root["HCaptcha_SiteKey"].toString());
        setLoginMessage("Additional authentication required", "error");
        setIsApiKeyRequired(true);
        setLoginStage(0);
        return;
    }

    if(root.contains("ErrorModel")){
        setLoginMessage(root["ErrorModel"].toObject()["Message"].toString(), "error");
        reset();
        return;
    }
    if(root.contains("error_description")) {
        setLoginMessage(root["error_description"].toString(), "error");
        reset();
        return;
    }
    if(root.contains("error")) {
        setLoginMessage(root["error"].toString(), "error");
        reset();
        return;
    }
    if(!root.contains("access_token")){
        setLoginMessage("Invalid API response #2", "error");
        reset();
        return;
    }
    if(!root.contains("refresh_token")){
        setLoginMessage("Invalid API response #3", "error");
        reset();
        return;
    }

    if(authenticateReply->error() == QNetworkReply::ProtocolInvalidOperationError){
        setLoginMessage("[" + QString::number(authenticateReply->error()) + "]" + authenticateReply->errorString(), "error");
        reset();
        return;
    }

    if(!root.contains("Kdf")){
        setLoginMessage("Invalid API response #4", "error");
        reset();
        return;
    }

    if(!root.contains("KdfIterations")){
        setLoginMessage("Invalid API response #5", "error");
        reset();
        return;
    }

    tokenService->setTokens(root["access_token"].toString(), root["refresh_token"].toString());
    user->fillOnAutheticate(tokenService->getUserIdFromToken(), tokenService->getEmailFromToken(), static_cast<KdfType>(root["Kdf"].toInt()), root["KdfIterations"].toInt());
    crypto->setKey(authentication->getKey());
    crypto->setHashedPassword(authentication->getHashedPassword());

    authentication->clear();
    setLoginStage(4);
    setLoginMessage("");
}
