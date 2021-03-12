#ifndef APP_H
#define APP_H

#include <QObject>
#include <QSettings>
#include <QUuid>

class AppIdService : public QObject
{
    Q_OBJECT
public:
    AppIdService(QSettings *settings);
    QString getAppId();

signals:

private:
    QSettings *settings;

};

#endif // APP_H
