#ifndef SYNCTASK_H
#define SYNCTASK_H

#include <QObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "apiauthtask.h"
#include "src/apijsondumper.h"
#include "src/factories/cipherfactory.h"
#include "src/factories/folderfactory.h"
#include "src/services/cryptoservice.h"
#include "src/services/stateservice.h"
#include "src/user.h"

class SyncTask : public ApiAuthTask
{
    Q_OBJECT
public:
    explicit SyncTask(User *user,
                      ApiJsonDumper *apiJsonDumper,
                      CryptoService *cryptoService,
                      StateService *stateService,
                      CipherFactory *cipherFactory,
                      FolderFactory *folderFactory,
                      Api *api,
                      TokenService *tokenService,
                      QObject *parent = nullptr);

    void exec() override;
    QNetworkReply *makeRequest() override;

protected:
    void apiCallSuccess() override;

private:
    User *user;
    ApiJsonDumper *apiJsonDumper;
    CryptoService *cryptoService;
    StateService *stateService;
    CipherFactory *cipherFactory;
    FolderFactory *folderFactory;
    bool syncProfile(QJsonObject profile);
    bool syncFolders(QString userId, QJsonArray folders);
    bool syncCollections();
    bool syncCiphers(QString userId, QJsonArray ciphers);
    bool syncSends();
    bool syncSettings();
    bool syncPolicies();

};

#endif // SYNCTASK_H
