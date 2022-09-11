#include "api.h"

Api::Api(QSettings *settings):
    settings(settings)
{
    apiUrl = settings->value("apiUrl", "https://api.bitwarden.com").toString();
    identityUrl = settings->value("identityUrl", "https://identity.bitwarden.com").toString();
    networkManager = new QNetworkAccessManager();
}

QString Api::getApiUrl()
{
    return apiUrl;
}

void Api::setApiUrl(const QString &value)
{
    if(apiUrl == value){
        return;
    }
    apiUrl = value;
    emit apiUrlChanged();
    settings->setValue("apiUrl", value);
    settings->sync();
}

QString Api::getIdentityUrl()
{
    return identityUrl;
}

void Api::setIdentityUrl(const QString &value)
{
    if(identityUrl == value){
        return;
    }
    identityUrl = value;
    emit identityUrlChanged();
    settings->setValue("identityUrl", value);
    settings->sync();
}

QString Api::getLastError()
{
    return lastError;
}

bool Api::getRequestRunning()
{
    return requestRunning;
}

QNetworkReply *Api::postPrelogin(QString email)
{
    QNetworkRequest request = buildRequest(QUrl(getApiUrl() + "/accounts/prelogin"));
    request.setRawHeader(QByteArray("Accept"), QByteArray("application/json"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
    QJsonObject jsonBody{ {"email", email} };
    reply = post(request, QJsonDocument(jsonBody).toJson(QJsonDocument::Compact));
    connect(reply, &QNetworkReply::finished, this, &Api::replyFinished);

    return reply;
}

QNetworkReply *Api::postIdentityToken(QByteArray body, QString email)
{
    QNetworkRequest request = buildRequest(getIdentityUrl() + "/connect/token");
    request.setRawHeader(QByteArray("Accept"), QByteArray("application/json"));
    request.setRawHeader(QByteArray("Auth-Email"), prepareAuthEmailHeaderValue(email));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=utf-8");
    reply = post(request, body);
    connect(reply, &QNetworkReply::finished, this, &Api::replyFinished);

    return reply;
}

QNetworkReply *Api::refreshAccessToken(QString userIdFromToken, QString refreshToken)
{
    QNetworkRequest request = buildRequest(QUrl(getIdentityUrl() + "/connect/token"));
    request.setRawHeader(QByteArray("Accept"), QByteArray("application/json"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=utf-8");

    QUrlQuery query;
    query.addQueryItem("grant_type", "refresh_token");
    query.addQueryItem("client_id", userIdFromToken);
    query.addQueryItem("refresh_token", refreshToken);

    QByteArray body;
    body.append(query.toString(QUrl::FullyEncoded));
    qDebug() << "request body: " << body;
    reply = post(request, body);
    connect(reply, &QNetworkReply::finished, this, &Api::replyFinished);

    return reply;
}

QNetworkReply *Api::getSync(QString accessToken)
{
    QNetworkRequest request = buildRequest(QUrl(getApiUrl() + "/sync?excludeDomains=true"));
    request.setRawHeader(QByteArray("Accept"), QByteArray("application/json"));
    request.setRawHeader(QByteArray("Authorization"), QByteArray("Bearer " + accessToken.toLatin1()));
    reply = get(request);
    connect(reply, &QNetworkReply::finished, this, &Api::replyFinished);

    return reply;
}

QNetworkReply *Api::postAccountVerifyPassword(QString masterPasswordHash, QString accessToken)
{
    QNetworkRequest request = buildRequest(QUrl(getApiUrl() + "/accounts/verify-password"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
    request.setRawHeader(QByteArray("Authorization"), QByteArray("Bearer " + accessToken.toLatin1()));
    QJsonObject jsonBody{ {"masterPasswordHash", masterPasswordHash} };
    reply = post(request, QJsonDocument(jsonBody).toJson(QJsonDocument::Compact));
    connect(reply, &QNetworkReply::finished, this, &Api::replyFinished);

    return reply;
}

QNetworkRequest Api::buildRequest(QUrl url)
{
    QNetworkRequest request(url);
    QByteArray deviceType;
    deviceType.setNum(static_cast<int>(DeviceType::LinuxDesktop));
    request.setRawHeader(QByteArray("Device-Type"), deviceType);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Bitwarden_CLI/1.15.0 (LINUX)");
    return request;
}

QByteArray Api::prepareAuthEmailHeaderValue(QString email)
{
    email.replace("+", "-").replace("/", "_").replace("=", "");
    QByteArray value;
    value.append(email);

    return value.toBase64();
}

QNetworkReply *Api::get(QNetworkRequest request)
{
    qDebug() << "GET request to: " << request.url().toEncoded();

    requestRunning = true;
    emit requestRunningChanged();
    lastError = "";
    emit lastErrorChanged();

    return networkManager->get(request);
}

QNetworkReply *Api::post(QNetworkRequest request, QByteArray body)
{
    qDebug() << "POST request to: " << request.url().toEncoded();
//    QList<QByteArray>::const_iterator i;
//    for (i = request.rawHeaderList().constBegin(); i != request.rawHeaderList().constEnd(); ++i)
//        qDebug() << "header:" << (*i) << ":" << request.rawHeader((*i));
//    qDebug() << "body: " << body;

    requestRunning = true;
    emit requestRunningChanged();
    lastError = "";
    emit lastErrorChanged();

    return networkManager->post(request, body);
}

void Api::replyFinished()
{
    qDebug() << "Finished request with code"
             << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString()
             << "from: "
             << reply->url().toEncoded();
//    QList<QPair<QByteArray, QByteArray>>::const_iterator i;
//    for (i = reply->rawHeaderPairs().begin(); i != reply->rawHeaderPairs().end(); ++i)
//        qDebug() << "header:" << (*i).first << ":" << (*i).second;
    requestRunning = false;
    emit requestRunningChanged();
    if(reply->error() != QNetworkReply::NoError){
        lastError = "[" + QString::number(reply->error()) + "] " + reply->errorString();
        emit lastErrorChanged();
    }
}



