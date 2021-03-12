#include "tokenservice.h"

TokenService::TokenService(QSettings *settings) : QObject(nullptr), settings(settings)
{
    if(settings->contains("accessToken") && settings->contains("refreshToken")){
        this->accessToken = settings->value("accessToken").toString();
        this->accessToken = settings->value("refreshToken").toString();
    }
}

void TokenService::setTokens(QString accessToken, QString refreshToken)
{
    this->accessToken = accessToken;
    this->refreshToken = refreshToken;
    persistTokens();
}

QString TokenService::getUserIdFromToken()
{
    QJsonObject decodedToken = decodeAccessToken();
    return decodedToken["sub"].toString();
}

QString TokenService::getEmailFromToken()
{
    QJsonObject decodedToken = decodeAccessToken();
    return decodedToken["email"].toString();
}

bool TokenService::exists()
{
    return accessToken != "" && refreshToken != "";
}

void TokenService::persistTokens()
{
    settings->setValue("accessToken", accessToken);
    settings->setValue("refreshToken", refreshToken);
}

QJsonObject TokenService::decodeAccessToken()
{
    QStringList tokenParts = accessToken.split(".");
    QByteArray token;
    token.append(tokenParts[1]);
    token = QByteArray::fromBase64(token);
    QJsonDocument decodedToken = QJsonDocument::fromJson(token);

    return decodedToken.object();
}
