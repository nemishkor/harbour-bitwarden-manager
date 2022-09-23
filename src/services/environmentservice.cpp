#include "environmentservice.h"

EnvironmentService::EnvironmentService(QSettings *settings):
    settings(settings)
{
    apiUrl = settings->value("apiUrl", "https://api.bitwarden.com").toString();
    identityUrl = settings->value("identityUrl", "https://identity.bitwarden.com").toString();
}

QString EnvironmentService::getApiUrl()
{
    return apiUrl;
}

void EnvironmentService::setApiUrl(const QString &newApiUrl)
{
    if(apiUrl == newApiUrl){
        return;
    }
    apiUrl = newApiUrl;
    emit apiUrlChanged();
    settings->setValue("apiUrl", newApiUrl);
    settings->sync();
}

QString EnvironmentService::getIdentityUrl()
{
    return identityUrl;
}

void EnvironmentService::setIdentityUrl(const QString &newIdentityUrl)
{
    if(identityUrl == newIdentityUrl){
        return;
    }
    identityUrl = newIdentityUrl;
    emit identityUrlChanged();
    settings->setValue("identityUrl", newIdentityUrl);
    settings->sync();
}
