#include "synctask.h"

SyncTask::SyncTask(User *user,
                   ApiJsonDumper *apiJsonDumper,
                   CryptoService *cryptoService,
                   StateService *stateService,
                   CipherFactory *cipherFactory,
                   FolderFactory *folderFactory,
                   Api *api,
                   TokenService *tokenService,
                   QObject *parent) :
    ApiAuthTask(api, tokenService, parent),
    user(user),
    apiJsonDumper(apiJsonDumper),
    cryptoService(cryptoService),
    stateService(stateService),
    cipherFactory(cipherFactory),
    folderFactory(folderFactory)
{
}

void SyncTask::exec()
{
    if(user->getUserId() == ""){
        emit fail("Can not sync. Not authorized! User is empty");
        return;
    }
    ApiAuthTask::exec();
}

QNetworkReply *SyncTask::makeRequest()
{
    emit newStage("Downloading");
    return api->getSync(tokenService->getAccessToken());
}

void SyncTask::apiCallSuccess()
{
    emit newStage("Processing downloaded data");

    try {
        QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());

        qDebug() << jsonDocument.toJson(QJsonDocument::Indented);

        if(!jsonDocument.isObject()){
            apiCallFail("Invalid sync API response #1");
            return;
        }

        QString userId = user->getUserId();
        QJsonObject root = jsonDocument.object();
        apiJsonDumper->dumpSyncFields(&root);

        emit newStage("profile");
        if(!syncProfile(root["profile"].toObject())){
            return;
        }

        emit newStage("folders");
        if(!syncFolders(userId, root["folders"].toArray())){
            return;
        }

        emit newStage("collections");
        if(!syncCollections()){
            return;
        }

        emit newStage("ciphers");
        if(!syncCiphers(userId, root["ciphers"].toArray())){
            return;
        }

        emit newStage("sends");
        if(!syncSends()){
            return;
        }

        emit newStage("settings");
        if(!syncSettings()){
            return;
        }

        emit newStage("policies");
        syncPolicies();

        emit newStage("");

    } catch (std::exception &e) {
        QString errorMessage;
        errorMessage.append("Sync failed: ").append(e.what());
        qWarning() << errorMessage;
        apiCallFail(errorMessage);
        return;
    }

    qDebug() << "sync done";
    ApiAuthTask::apiCallSuccess();
}

bool SyncTask::syncProfile(QJsonObject profile)
{
    apiJsonDumper->dumpProfileFields(&profile);
    QString stamp = user->getStamp();
    if(stamp != "" && stamp != profile["securityStamp"].toString()){
        apiCallFail("Stamp has changed. Logout is required");
        return false;
    }

    cryptoService->setEncKey(profile["key"].toString());
    // TODO: ?
    cryptoService->setPrivateKey(profile["privateKey"].toString());

    user->setStamp(profile["securityStamp"].toString());
    user->setEmail(profile["email"].toString());
    user->setName(profile["name"].toString());
    user->setPremium(profile["premium"].toBool());

    qDebug() << "profile was updated";
    // TODO sync user's organizations
    return true;
}

bool SyncTask::syncFolders(QString userId, QJsonArray folders)
{
    QJsonObject apiFolder;
    QJsonArray::const_iterator i;
    for (i = folders.constBegin(); i != folders.constEnd(); i++){
        apiFolder = (*i).toObject();
        Folder folder = folderFactory->create(apiFolder, userId);
        stateService->add(folder);
    }
    return true;
}

bool SyncTask::syncCollections()
{
    return true;
}

bool SyncTask::syncCiphers(QString userId, QJsonArray ciphers)
{
    QJsonArray::const_iterator i;
    QJsonObject c;

    for (i = ciphers.constBegin(); i != ciphers.constEnd(); i++){
        qDebug() << "Add cipher";
        c = (*i).toObject();
        Cipher cipher = cipherFactory->create(c, userId);
        stateService->add(cipher);
    }

    qDebug() << "Ciphers were added";
    return true;
}

bool SyncTask::syncSends()
{
    return true;
}

bool SyncTask::syncSettings()
{
    return true;
}

bool SyncTask::syncPolicies()
{
    return true;
}
