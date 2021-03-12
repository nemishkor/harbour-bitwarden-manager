#include "auth.h"

Auth::Auth(AppIdService *appIdService, Api *api, CryptoService *crypto) :
    appIdService(appIdService),
    api(api),
    crypto(crypto),
    email("")
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
    loginMessage = "Checking encoding settings";
    emit loginMessageChanged();
    QJsonObject jsonBody{ {"email", email} };
    preloginReply = api->postPrelogin(QUrl(api->getApiUrl() + "/accounts/prelogin"), QJsonDocument(jsonBody).toJson(QJsonDocument::Compact));
    connect(preloginReply, &QNetworkReply::finished, this, &Auth::makePreloginKey);
}

void Auth::makePreloginKey()
{

    loginMessage = "Credentials encoding";
    emit loginMessageChanged();

    QJsonDocument json = QJsonDocument::fromJson(preloginReply->readAll());
//    qDebug() << "json response:" << json.toJson(QJsonDocument::Compact);
    KdfType kdfType = static_cast<KdfType>(json.object()["Kdf"].toInt());
    QByteArray key = crypto->makeKey(password, email, kdfType, json.object()["KdfIterations"].toInt());
    hashedPassword = crypto->hashPassword(key, password);
    password = ""; // we don't need raw password anymore

    loginMessage = "Logging in";
    emit loginMessageChanged();

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
    QJsonDocument json = QJsonDocument::fromJson(identityReply->readAll());
    qDebug() << "json response:" << json.toJson(QJsonDocument::Compact);
    if(json.isObject()){
        QJsonObject responseObj = json.object();
        if(responseObj.contains("ErrorModel")){
            loginMessage = responseObj["ErrorModel"].toObject()["Message"].toString();
            emit loginMessageChanged();
            loginMessageType = "error";
            emit loginMessageTypeChanged();
        } else if(responseObj.contains("error_description")) {
            loginMessage = responseObj["error_description"].toString();
            emit loginMessageChanged();
            loginMessageType = "error";
            emit loginMessageTypeChanged();
        } else if(responseObj.contains("error")) {
            loginMessage = responseObj["error"].toString();
            emit loginMessageChanged();
            loginMessageType = "error";
            emit loginMessageTypeChanged();
        } else {
            loginMessage = "";
            emit loginMessageChanged();
        }
    } else {
        loginMessage = "Invalid API response";
        emit loginMessageChanged();
        loginMessageType = "error";
        emit loginMessageTypeChanged();
    }
    loginProcessing = false;
    emit loginProcessingChanged();
}
