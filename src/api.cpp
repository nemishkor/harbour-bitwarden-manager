#include "api.h"

Api::Api(QSettings *settings, EnvironmentService *environmentService):
    settings(settings),
    environmentService(environmentService)
{
    networkManager = new QNetworkAccessManager();
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
    QNetworkRequest request = buildRequest(QUrl(environmentService->getIdentityUrl() + "/accounts/prelogin"));
    request.setRawHeader(QByteArray("Accept"), QByteArray("application/json"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
    QJsonObject jsonBody{ {"email", email} };
    reply = post(request, QJsonDocument(jsonBody).toJson(QJsonDocument::Compact));
    connect(reply, &QNetworkReply::finished, this, &Api::replyFinished);

    return reply;
}

QNetworkReply *Api::postIdentityToken(QByteArray body, QString email)
{
    QNetworkRequest request = buildRequest(environmentService->getIdentityUrl() + "/connect/token");
    request.setRawHeader(QByteArray("Accept"), QByteArray("application/json"));
    request.setRawHeader(QByteArray("Auth-Email"), prepareAuthEmailHeaderValue(email));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=utf-8");
    reply = post(request, body);
    connect(reply, &QNetworkReply::finished, this, &Api::replyFinished);

    return reply;
}

QNetworkReply *Api::refreshAccessToken(QString userIdFromToken, QString refreshToken)
{
    QNetworkRequest request = buildRequest(QUrl(environmentService->getIdentityUrl() + "/connect/token"));
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
    QNetworkRequest request = buildRequest(QUrl(environmentService->getApiUrl() + "/sync?excludeDomains=true"));
    request.setRawHeader(QByteArray("Accept"), QByteArray("application/json"));
    request.setRawHeader(QByteArray("Authorization"), QByteArray("Bearer " + accessToken.toLatin1()));
    reply = get(request);
    connect(reply, &QNetworkReply::finished, this, &Api::replyFinished);

    return reply;
}

QNetworkReply *Api::postAccountVerifyPassword(QString masterPasswordHash, QString accessToken)
{
    QNetworkRequest request = buildRequest(QUrl(environmentService->getApiUrl() + "/accounts/verify-password"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
    request.setRawHeader(QByteArray("Authorization"), QByteArray("Bearer " + accessToken.toLatin1()));
    QJsonObject jsonBody{ {"masterPasswordHash", masterPasswordHash} };
    reply = post(request, QJsonDocument(jsonBody).toJson(QJsonDocument::Compact));
    connect(reply, &QNetworkReply::finished, this, &Api::replyFinished);

    return reply;
}

QNetworkReply *Api::postFolder(QString name, QString accessToken)
{
    QNetworkRequest request = buildRequest(QUrl(environmentService->getApiUrl() + "/folders"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
    request.setRawHeader(QByteArray("Authorization"), QByteArray("Bearer " + accessToken.toLatin1()));
    QJsonObject jsonBody{ {"name", name} };
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
    request.setHeader(QNetworkRequest::UserAgentHeader, "Bitwarden_CLI/1.22.1 (LINUX)");
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
    request.setRawHeader("Bitwarden-Client-Name", "cli");
    request.setRawHeader("Bitwarden-Client-Version", "1.22.1");
    logRequest("POST", &request, &body);

    requestRunning = true;
    emit requestRunningChanged();
    lastError = "";
    emit lastErrorChanged();

    return networkManager->post(request, body);
}

void Api::replyFinished()
{
    logReply(reply);
    requestRunning = false;
    emit requestRunningChanged();
    if(reply->error() != QNetworkReply::NoError){
        lastError = "[" + QString::number(reply->error()) + "] " + reply->errorString();
        emit lastErrorChanged();
    }
}

void Api::logRequest(QString method, QNetworkRequest *request, QByteArray *body)
{
    QString message;
    message.append("Request <<<\r\n")
           .append(QDateTime::currentDateTimeUtc().toString(Qt::ISODate))
           .append("\r\n")
           .append(method)
           .append(" ")
           .append(request->url().toEncoded())
           .append("\r\nHeaders:\r\n");

    for (int i = 0; i < request->rawHeaderList().size(); i++)
        message.append(request->rawHeaderList()[i])
               .append(":")
               .append(request->rawHeader(request->rawHeaderList()[i]))
               .append("\r\n");

    message.append("Body:\r\n")
           .append(*body)
           .append("\r\n>>>");

    qDebug().nospace().noquote() << message;
}

void Api::logReply(QNetworkReply *reply)
{
    QString message;
    message.append("Response <<<\r\n")
           .append(QDateTime::currentDateTimeUtc().toString(Qt::ISODate))
           .append("\r\n")
           .append(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString())
           .append(" ")
           .append(reply->url().toEncoded())
           .append("\r\nHeaders:\r\n");

    for (int i = 0; i < reply->rawHeaderList().size(); i++)
        message.append(reply->rawHeaderList()[i])
               .append(":")
               .append(reply->rawHeader(reply->rawHeaderList()[i]))
               .append("\r\n");

    message.append(">>>");

    qDebug().nospace().noquote() << message;
}

