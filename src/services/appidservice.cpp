#include "appidservice.h"

AppIdService::AppIdService(QSettings *settings, QObject* parent) : QObject(parent), settings(settings)
{
}

QString AppIdService::getAppId()
{
    if(settings->contains("appId")){
        return settings->value("appId").toString();
    }
    QString uuidStr = QUuid::createUuid().toString();
    // remove "{" and "}"
    uuidStr = uuidStr.mid(1, uuidStr.length() - 2);
    settings->setValue("appId", uuidStr);
    settings->sync();

    return uuidStr;
}
