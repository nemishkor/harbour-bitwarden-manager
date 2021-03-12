#include "auth.h"

Auth::Auth(AppIdService *appIdService, TokenService *tokenService, Api *api, CryptoService *crypto, User *user) :
    appIdService(appIdService),
    tokenService(tokenService),
    api(api),
    crypto(crypto),
    user(user),
    email("nemish94@gmail.com")
{

}

QString Auth::getEmail()
{
    return email;
}

bool Auth::isLoginProcessing()
{
    return loginProcessing;
}

QString Auth::getLoginMessage()
{
    return loginMessage;
}

QString Auth::getLoginMessageType()
{
    return loginMessageType;
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

void Auth::login(QString password)
{
    loginProcessing = true;
    emit loginProcessingChanged();
    loginMessage = "";
    emit loginMessageChanged();
    loginMessageType = "info";
    emit loginMessageTypeChanged();

    this->password = password;
    prelogin();
}

void Auth::prelogin()
{
    setLoginMessage("Checking encoding settings");
    QJsonObject jsonBody{ {"email", email} };
    preloginReply = api->postPrelogin(QUrl(api->getApiUrl() + "/accounts/prelogin"), QJsonDocument(jsonBody).toJson(QJsonDocument::Compact));
    connect(preloginReply, &QNetworkReply::finished, this, &Auth::makePreloginKey);
}

void Auth::makePreloginKey()
{
    setLoginMessage("Credentials encoding");

    QJsonDocument json = QJsonDocument::fromJson(preloginReply->readAll());
//    qDebug() << "json response:" << json.toJson(QJsonDocument::Compact);
    KdfType kdfType = static_cast<KdfType>(json.object()["Kdf"].toInt());
    QByteArray key = crypto->makeKey(password, email, kdfType, json.object()["KdfIterations"].toInt());
    hashedPassword = crypto->hashPassword(key, password);
    password = ""; // we don't need raw password anymore

    setLoginMessage("Logging in");

    // postIdentityToken
    identityReply = api->postIdentityToken(api->getIdentityUrl() + "/connect/token", makeIdentityTokenRequestBody());
    connect(identityReply, &QNetworkReply::finished, this, &Auth::identityReplyFinished);
}

QByteArray Auth::makeIdentityTokenRequestBody()
{
    QUrlQuery query;
    query.addQueryItem("scope", "api offline_access");
    query.addQueryItem("client_id", "cli");
    query.addQueryItem("grant_type", "password"); // auth via password
    query.addQueryItem("username", QUrl::toPercentEncoding(email));
    query.addQueryItem("password", QUrl::toPercentEncoding(hashedPassword, "%"));
    query.addQueryItem("deviceType", QString::number(static_cast<int>(DeviceType::LinuxDesktop)));
    query.addQueryItem("deviceIdentifier", appIdService->getAppId());
    query.addQueryItem("deviceName", "linux"); // TODO: try to change to real device name

    QByteArray body;
    body.append(query.toString(QUrl::FullyEncoded));

    return body;
}

void Auth::identityReplyFinished()
{
    qDebug() << "Auth::identityReplyFinished";

    loginProcessing = false;
    emit loginProcessingChanged();

    QJsonDocument json = QJsonDocument::fromJson(identityReply->readAll());
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

    setLoginMessage("");

    tokenService->setTokens(root["access_token"].toString(), root["refresh_token"].toString());
    user->setInformation(tokenService->getUserIdFromToken(), tokenService->getEmailFromToken(), static_cast<KdfType>(root["Kdf"].toInt()), root["KdfIterations"].toInt());
}
