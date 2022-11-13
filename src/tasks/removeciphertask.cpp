#include "removeciphertask.h"

RemoveCipherTask::RemoveCipherTask(
        CipherService *cipherService,
        QString id,
        TokenService *tokenService,
        Api *api,
        QObject *parent)
    : ApiAuthTask(api, tokenService, parent),
      cipherService(cipherService),
      id(id)
{
}

void RemoveCipherTask::exec()
{
    index = cipherService->removeFromView(id);
    if(index.isValid()){
        ApiAuthTask::exec();
    } else {
        qWarning() << "Unable to remove cipher from the view" << id;
    }
}

const QString &RemoveCipherTask::getId() const
{
    return id;
}

void RemoveCipherTask::apiCallSuccess()
{
    cipherService->removeFromState(id);
    ApiAuthTask::apiCallSuccess();
}

void RemoveCipherTask::apiCallFail(const QString &message)
{
    cipherService->decryptOne(id, index);
    ApiAuthTask::apiCallFail(message);
}

QNetworkReply *RemoveCipherTask::makeRequest()
{
    return api->removeCipher(id, tokenService->getAccessToken());
}
