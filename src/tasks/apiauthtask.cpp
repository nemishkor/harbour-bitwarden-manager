#include "apiauthtask.h"

ApiAuthTask::ApiAuthTask(Api *api,
                         TokenService *tokenService,
                         QObject *parent)
    : ApiTask(api, parent),
      tokenService(tokenService)
{

}

void ApiAuthTask::exec()
{
    if(!tokenService->exists()){
        apiCallFail("Not authorized! Not found tokens");
        return;
    }
    connect(tokenService, &TokenService::refreshTokenSuccess, this, &ApiAuthTask::refreshTokenSuccess);
    connect(tokenService, &TokenService::refreshTokenFail, this, &ApiAuthTask::refreshTokenFail);
    tokenService->validateToken();
}

void ApiAuthTask::refreshTokenSuccess()
{
    disconnect(tokenService, &TokenService::refreshTokenSuccess, this, &ApiAuthTask::refreshTokenSuccess);
    disconnect(tokenService, &TokenService::refreshTokenFail, this, &ApiAuthTask::refreshTokenFail);
    ApiTask::exec();
}

void ApiAuthTask::refreshTokenFail(QString reason)
{
    disconnect(tokenService, &TokenService::refreshTokenSuccess, this, &ApiAuthTask::refreshTokenSuccess);
    disconnect(tokenService, &TokenService::refreshTokenFail, this, &ApiAuthTask::refreshTokenFail);
    apiCallFail(reason);
}
