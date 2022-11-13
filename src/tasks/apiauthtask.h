#ifndef APIAUTHTASK_H
#define APIAUTHTASK_H

#include <QObject>

#include "apitask.h"
#include "src/services/tokenservice.h"

class ApiAuthTask : public ApiTask
{
    Q_OBJECT
public:
    explicit ApiAuthTask(Api *api,
                         TokenService *tokenService,
                         QObject *parent = nullptr);
    void exec() override;

protected:
    TokenService *tokenService;

private slots:
    void refreshTokenSuccess();
    void refreshTokenFail(QString reason);

};

#endif // APIAUTHTASK_H
