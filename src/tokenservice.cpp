#include "tokenservice.h"

TokenService::TokenService(QSettings *settings, Api*api) :
    QObject(nullptr),
    settings(settings),
    api(api)
{
    if(settings->contains("accessToken") && settings->contains("refreshToken")){
        setAccessToken(settings->value("accessToken").toString());
        setRefreshToken(settings->value("refreshToken").toString());
    }
}

void TokenService::setTokens(QString accessToken, QString refreshToken)
{
    setAccessToken(accessToken);
    setRefreshToken(refreshToken);
    settings->sync();
}

QString TokenService::getUserIdFromToken()
{
    return decodeAccessToken()["sub"].toString();
}

QString TokenService::getEmailFromToken()
{
    return decodeAccessToken()["email"].toString();
}

QString TokenService::getClientIdFromToken()
{
    return decodeAccessToken()["client_id"].toString();
}

bool TokenService::exists()
{
    return !accessToken.isEmpty() && !accessToken.isNull() && !refreshToken.isEmpty() && !refreshToken.isNull();
}

QString TokenService::getAccessToken() const
{
    return accessToken;
}

bool TokenService::tokenNeedsRefresh(int minutes)
{
    quint32 expiration = getTokenExpirationSec();
    quint32 nowEpochSec = QDateTime::currentMSecsSinceEpoch() / 1000;
    int secondsRemaining = expiration - nowEpochSec;
    qDebug() << "token seconds remaining" << secondsRemaining;
    return secondsRemaining < (60 * minutes);
}

void TokenService::clearTokens()
{
    qDebug() << "clear tokens";
    accessToken.clear();
    settings->remove("accessToken");
    refreshToken.clear();
    settings->remove("refreshToken");
    settings->sync();
}

void TokenService::validateToken()
{
    if(!tokenNeedsRefresh()){
        qDebug() << "Token doesn't need refreshing";
        emit refreshTokenSuccess();
        return;
    }

    if(tokenIsRefreshing){
        qWarning() << "Refreshing token in progress already";
        emit refreshTokenFail("Refreshing token in progress already");
        return;
    }

    tokenIsRefreshing = true;

    if(refreshToken == ""){
        tokenIsRefreshing = false;
        qCritical() << "Oops. Something went wrong. Can not refresh access token. Refresh token is empty";
        throw new QException();
    }

    qDebug() << "Refreshing token...";
    refreshTokenReply = api->refreshAccessToken(getClientIdFromToken(), refreshToken);
    connect(refreshTokenReply, &QNetworkReply::finished, this, &TokenService::refreshTokenReplyFinished);
}

void TokenService::abort()
{
    if(refreshTokenReply && refreshTokenReply->isRunning()){
        refreshTokenReply->abort();
    }
}

void TokenService::setAccessToken(const QString &value)
{
    if(accessToken != value){
        accessToken = value;
        qDebug().nospace().noquote() << "Set accessToken: " << accessToken;
        settings->setValue("accessToken", accessToken);
    }
}

QString TokenService::getRefreshToken() const
{
    return refreshToken;
}

void TokenService::setRefreshToken(const QString &value)
{
    if(refreshToken != value){
        refreshToken = value;
        qDebug().nospace().noquote() << "Set refreshToken: " << refreshToken;
        settings->setValue("refreshToken", refreshToken);
    }
}

QJsonObject TokenService::decodeAccessToken()
{
    QStringList tokenParts = accessToken.split(".");
    QByteArray token;
    token.append(tokenParts[1]);
    token = QByteArray::fromBase64(token);
    QJsonDocument decodedToken = QJsonDocument::fromJson(token);

    qDebug() << "Decoded access token:" << decodedToken.toJson(QJsonDocument::Compact);

    return decodedToken.object();
}

quint32 TokenService::getTokenExpirationSec()
{
    QJsonObject decodedToken = decodeAccessToken();
    quint32 expSec = decodedToken["exp"].toInt();
    return expSec;
}

void TokenService::refreshTokenReplyFinished()
{
    if(refreshTokenReply->error() != QNetworkReply::NoError &&
            refreshTokenReply->error() != QNetworkReply::ProtocolInvalidOperationError){
        emit refreshTokenFail("Token refreshing failed. " + getFailedReplyMessage(refreshTokenReply));
        return;
    }

    QJsonDocument json = QJsonDocument::fromJson(refreshTokenReply->readAll());

    if(!json.isObject()){
        emit refreshTokenFail("Invalid refresh token API response #1");
        return;
    }

    QJsonObject root = json.object();

    if(refreshTokenReply->error() == QNetworkReply::ProtocolInvalidOperationError && root.contains("error")){
        emit refreshTokenFail("Token refreshing failed. " + root["error"].toString());
        return;
    }

    if(!root.contains("access_token")){
        emit refreshTokenFail("Token refreshing failed. API response does not contain access_token field");
        return;
    }

    if(!root.contains("refresh_token")){
        emit refreshTokenFail("Token refreshing failed. API response does not contain access_token field");
        return;
    }

    if(refreshTokenReply->error() == QNetworkReply::ProtocolInvalidOperationError){
        emit refreshTokenFail(getFailedReplyMessage(refreshTokenReply));
        return;
    }

    setTokens(root["access_token"].toString(), root["refresh_token"].toString());
    emit refreshTokenSuccess();
}

QString TokenService::getFailedReplyMessage(QNetworkReply *failedReply)
{
    return "API: [" + QString::number(failedReply->error()) + "]" + failedReply->errorString();
}
