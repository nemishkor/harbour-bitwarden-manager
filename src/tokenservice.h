#ifndef TOKENSERVICE_H
#define TOKENSERVICE_H

#include <QByteArray>
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

private:
    QString accessToken;
    QString refreshToken;
    QSettings *settings;
    void persistTokens();
    QJsonObject decodeAccessToken();

signals:

};

#endif // TOKENSERVICE_H
