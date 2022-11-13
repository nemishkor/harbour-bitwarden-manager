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
#include "services/environmentservice.h"

class Api : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString lastError READ getLastError NOTIFY lastErrorChanged);
    Q_PROPERTY(bool requestRunning READ getRequestRunning NOTIFY requestRunningChanged);

public:
    Api(QSettings *settings, EnvironmentService *environmentService, QObject* parent = nullptr);

    QString getLastError();
    bool getRequestRunning();
    QNetworkReply *postPrelogin(QString email);
    QNetworkReply *postIdentityToken(QByteArray body, QString email);
    QNetworkReply *refreshAccessToken(QString userIdFromToken, QString refreshToken);
    QNetworkReply *getSync(QString accessToken);
    QNetworkReply *postAccountVerifyPassword(QString masterPasswordHash, QString accessToken);
    QNetworkReply *postFolder(QString name, QString accessToken);
    QNetworkReply *removeCipher(QString id, QString accessToken);
    static QString getFailedReplyMessage(QNetworkReply *failedReply);

private:
    QSettings *settings;
    EnvironmentService *environmentService;
    QNetworkAccessManager *networkManager;
    bool requestRunning = false;
    QString lastError;

    QNetworkRequest buildRequest(QUrl url);
    QByteArray prepareAuthEmailHeaderValue(QString email);
    QNetworkReply *get(QNetworkRequest request);
    QNetworkReply *post(QNetworkRequest request, QByteArray body);
    QNetworkReply *deleteResource(QNetworkRequest request);
    QNetworkReply *reply;
    void replyFinished();
    void logRequest(QString method, QNetworkRequest *request, QByteArray *body = nullptr);
    void logReply(QNetworkReply *reply);


signals:
    void lastErrorChanged();
    void requestRunningChanged();
};

#endif // API_H
