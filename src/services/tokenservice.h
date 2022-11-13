#ifndef TOKENSERVICE_H
#define TOKENSERVICE_H

#include "src/api.h"

#include <QByteArray>
#include <QDateTime>
#include <QDebug>
#include <QException>
#include <QtGlobal>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QSettings>
#include <QNetworkReply>

class TokenService : public QObject
{
    Q_OBJECT
public:
    explicit TokenService(QSettings *settings, Api *api, QObject* parent = nullptr);
    void setTokens(QString accessToken, QString refreshToken);
    QString getUserIdFromToken();
    QString getEmailFromToken();
    QString getClientIdFromToken();
    bool exists();
    QString getAccessToken() const;
    QString getRefreshToken() const;
    bool tokenNeedsRefresh(int minutes = 5);
    void clearTokens();
    void validateToken();
    void abort();

private:
    QString accessToken;
    void setAccessToken(const QString &value);
    QString refreshToken;
    void setRefreshToken(const QString &value);
    QSettings *settings;
    Api *api;
    void persistTokens();
    QJsonObject decodeAccessToken();
    quint32 getTokenExpirationSec();
    bool tokenIsRefreshing = false;
    QNetworkReply *refreshTokenReply;
    void refreshTokenReplyFinished();
    QString getFailedReplyMessage(QNetworkReply *failedReply);

signals:
    void refreshTokenSuccess();
    void refreshTokenFail(QString reason);

};

#endif // TOKENSERVICE_H
