#ifndef APP_H
#define APP_H

#include <QObject>
#include <QSettings>
#include <QUuid>

class AppIdService : public QObject
{
    Q_OBJECT
public:
    explicit AppIdService(QObject *parent = nullptr);
    QString getAppId();

signals:

private:
    QSettings *settings;

};

#endif // APP_H
