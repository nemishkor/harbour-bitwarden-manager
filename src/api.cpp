#include "api.h"

Api::Api(QString apiUrl, QString identityUrl): apiUrl(apiUrl), identityUrl(identityUrl)
{
    networkManager = new QNetworkAccessManager();
}

QString Api::getApiUrl()
{
    return apiUrl;
}

QString Api::getIdentityUrl()
{
    return identityUrl;
}

QString Api::getLastError()
{
    return lastError;
}

bool Api::getRequestRunning()
{
    return requestRunning;
}

QNetworkReply *Api::postPrelogin(QUrl url, QByteArray body)
{
    QNetworkRequest request = buildRequest(url);
    request.setRawHeader(QByteArray("Accept"), QByteArray("application/json"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
    reply = post(request, body);
    connect(reply, &QNetworkReply::finished, this, &Api::replyFinished);

    return reply;
}

QNetworkReply *Api::postIdentityToken(QUrl url, QByteArray body)
{
    QNetworkRequest request = buildRequest(url);
    request.setRawHeader(QByteArray("Accept"), QByteArray("application/json"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=utf-8");
    reply = post(request, body);
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
