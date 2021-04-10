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
#include <QSettings>
#include <QString>
#include <QUrl>
#include <QUrlQuery>

#include "devicetype.h"
#include "kdftype.h"
#include "tokenservice.h"

class Api : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString apiUrl READ getApiUrl WRITE setApiUrl NOTIFY apiUrlChanged);
    Q_PROPERTY(QString identityUrl READ getIdentityUrl WRITE setIdentityUrl NOTIFY identityUrlChanged);
    Q_PROPERTY(QString lastError READ getLastError NOTIFY lastErrorChanged);
    Q_PROPERTY(bool requestRunning READ getRequestRunning NOTIFY requestRunningChanged);

public:
    Api(QSettings *settings);

    QString getApiUrl();
    void setApiUrl(const QString &value);

    QString getIdentityUrl();
    void setIdentityUrl(const QString &value);

    QString getLastError();
    bool getRequestRunning();
    QNetworkReply *postPrelogin(QString email);
    QNetworkReply *postIdentityToken(QByteArray body);
    QNetworkReply *refreshAccessToken(QString userIdFromToken, QString refreshToken);
    QNetworkReply *getSync(QString accessToken);
    QNetworkReply *postAccountVerifyPassword(QString masterPasswordHash, QString accessToken);

private:
    QSettings *settings;
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
    void apiUrlChanged();
    void identityUrlChanged();
    void lastErrorChanged();
    void requestRunningChanged();
};

#endif // API_H
