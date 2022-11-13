#ifndef REMOVECIPHERTASK_H
#define REMOVECIPHERTASK_H

#include "apiauthtask.h"
#include "src/api.h"
#include "src/viewmodels/cipherslistmodel.h"
#include "src/services/cipherservice.h"
#include "src/services/tokenservice.h"

class RemoveCipherTask : public ApiAuthTask
{
    Q_OBJECT
public:
    explicit RemoveCipherTask(CipherService *cipherService,
                              QString id,
                              TokenService *tokenService,
                              Api *api,
                              QObject *parent = nullptr);
    void exec() override;
    const QString &getId() const;

protected:
    void apiCallSuccess() override;
    void apiCallFail(const QString& message) override;

private:
    CipherService *cipherService;
    QString id;
    QModelIndex index;
    QNetworkReply *makeRequest() override;

};

#endif // REMOVECIPHERTASK_H
