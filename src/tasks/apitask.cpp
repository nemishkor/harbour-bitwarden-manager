#include "apitask.h"

ApiTask::ApiTask(Api *api,
                 QObject *parent)
    : QObject(parent), api(api)
{
}

void ApiTask::exec()
{
    reply = makeRequest();
    connect(reply, &QNetworkReply::finished, this, &ApiTask::requestFinish);
}

void ApiTask::requestFinish()
{
    if(reply->error() != QNetworkReply::NoError && reply->error() != QNetworkReply::ProtocolInvalidOperationError){
        apiCallFail(Api::getFailedReplyMessage(reply));
        return;
    }
    apiCallSuccess();
}

void ApiTask::apiCallSuccess()
{
    emit success();
}

void ApiTask::apiCallFail(const QString& message)
{
    emit fail(message);
}
