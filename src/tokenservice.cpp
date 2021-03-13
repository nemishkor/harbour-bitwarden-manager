#include "tokenservice.h"

TokenService::TokenService(QSettings *settings) : QObject(nullptr), settings(settings)
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
    return accessToken != "" && refreshToken != "";
}

QString TokenService::getAccessToken() const
{
    return accessToken;
}

bool TokenService::tokenNeedsRefresh(int minutes)
{
    quint32 expiration = getTokenExpirationSec();
    qDebug() << "token expiration epoch sec" << expiration;
    quint32 nowEpochSec = QDateTime::currentMSecsSinceEpoch() / 1000;
    qDebug() << "now epoch sec" << nowEpochSec;
    int secondsRemaining = expiration - nowEpochSec;
    qDebug() << "token seconds remaining" << secondsRemaining;
    return secondsRemaining < (60 * minutes);
}

void TokenService::setAccessToken(const QString &value)
{
    if(accessToken != value){
        accessToken = value;
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

    return decodedToken.object();
}

quint32 TokenService::getTokenExpirationSec()
{
    QJsonObject decodedToken = decodeAccessToken();
    quint32 expSec = decodedToken["exp"].toInt();
    qDebug() << "token exp" << expSec;
    return expSec;
}
