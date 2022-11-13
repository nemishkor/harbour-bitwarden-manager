#ifndef APPIDSERVICE_H
#define APPIDSERVICE_H

#include <QObject>
#include <QSettings>
#include <QUuid>

class AppIdService : public QObject
{
    Q_OBJECT
public:
    AppIdService(QSettings *settings, QObject* parent = nullptr);
    QString getAppId();

signals:

private:
    QSettings *settings;

};


#endif // APPIDSERVICE_H
