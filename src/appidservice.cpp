#include "appidservice.h"

AppIdService::AppIdService(QObject *parent) : QObject(parent)
{
    settings = new QSettings("harbour-bitwarden-manager");
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

    return uuidStr;
}
