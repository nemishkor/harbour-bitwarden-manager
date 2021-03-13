#ifndef API_H
#define API_H

#include <QByteArray>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QString>
#include <QUrl>
#include <QUrlQuery>

#include "devicetype.h"
#include "kdftype.h"
#include "tokenservice.h"

class Api : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString lastError READ getLastError NOTIFY lastErrorChanged);
    Q_PROPERTY(bool requestRunning READ getRequestRunning NOTIFY requestRunningChanged);

public:
    Api(QString apiUrl, QString identityUrl);
    QString getApiUrl();
    QString getIdentityUrl();
    QString getLastError();
    bool getRequestRunning();
    QNetworkReply *postPrelogin(QString email);
    QNetworkReply *postIdentityToken(QByteArray body);
    QNetworkReply *refreshAccessToken(QString userIdFromToken, QString refreshToken);
    QNetworkReply *getSync(QString accessToken);

private:
    QNetworkAccessManager *networkManager;
    QString apiUrl;
    QString identityUrl;
    bool requestRunning = false;
    QString lastError;

    QNetworkRequest buildRequest(QUrl url);
    QNetworkReply *get(QNetworkRequest request);
    QNetworkReply *post(QNetworkRequest request, QByteArray body);
    QNetworkReply *reply;
    void replyFinished();

signals:
    void lastErrorChanged();
    void requestRunningChanged();
};

#endif // API_H
