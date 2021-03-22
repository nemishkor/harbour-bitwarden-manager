#ifndef TOKENSERVICE_H
#define TOKENSERVICE_H

#include <QByteArray>
#include <QDateTime>
#include <QDebug>
#include <QtGlobal>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QSettings>

class TokenService : public QObject
{
    Q_OBJECT
public:
    explicit TokenService(QSettings *settings);
    void setTokens(QString accessToken, QString refreshToken);
    QString getUserIdFromToken();
    QString getEmailFromToken();
    QString getClientIdFromToken();
    bool exists();
    QString getAccessToken() const;
    QString getRefreshToken() const;
    bool tokenNeedsRefresh(int minutes = 5);
    void clearTokens();

private:
    QString accessToken;
    void setAccessToken(const QString &value);
    QString refreshToken;
    void setRefreshToken(const QString &value);
    QSettings *settings;
    void persistTokens();
    QJsonObject decodeAccessToken();
    quint32 getTokenExpirationSec();

signals:

};

#endif // TOKENSERVICE_H
