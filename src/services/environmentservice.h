#ifndef ENVIRONMENTSERVICE_H
#define ENVIRONMENTSERVICE_H

#include <QObject>
#include <QSettings>

class EnvironmentService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString apiUrl READ getApiUrl WRITE setApiUrl NOTIFY apiUrlChanged);
    Q_PROPERTY(QString identityUrl READ getIdentityUrl WRITE setIdentityUrl NOTIFY identityUrlChanged);

public:
    EnvironmentService(QSettings *settings);

    QString getApiUrl();
    void setApiUrl(const QString &value);

    QString getIdentityUrl();
    void setIdentityUrl(const QString &value);

private:
    QSettings *settings;
    QString apiUrl;
    QString identityUrl;

signals:
    void apiUrlChanged();
    void identityUrlChanged();
};

#endif // ENVIRONMENTSERVICE_H
