#include "vaultmanager.h"

VaultManager::VaultManager(CryptoService *cryptoService, User *user, Api *api, TokenService *tokenService) :
    QObject(nullptr),
    cryptoService(cryptoService),
    user(user),
    api(api),
    tokenService(tokenService)
{
    connect(cryptoService, &CryptoService::keyChanged, this, &VaultManager::keyChanged);
    refreshTokenReply = nullptr;
    verifyPasswordReply = nullptr;
}

void VaultManager::unlock(QString password)
{
    setUnlocking(true);
    setUnlockMessage("Encrypting");
    key = cryptoService->makeKey(password, user->getEmail(), user->getKdf(), user->getKdfIterations());
    masterPasswordHash = cryptoService->hashPassword(key, password);
    if(masterPasswordHash == cryptoService->getHashedPassword()){
        cryptoService->setKey(SymmetricCryptoKey(key));
        key.clear();
        masterPasswordHash.clear();
        setUnlocking(false);
        setUnlockMessage("");
        return;
    }

    if(user->getUserId() == "" || !tokenService->exists()){
        key.clear();
        masterPasswordHash.clear();
        setUnlockMessage("Can not sync. Not authorized!");
        setUnlocking(false);
        return;
    }

    if(tokenService->tokenNeedsRefresh()){
        setUnlockMessage("Token refreshing");
        QString refreshToken = tokenService->getRefreshToken();
        if(refreshToken == ""){
            key.clear();
            masterPasswordHash.clear();
            setUnlockMessage("Can not refresh access token. Refresh token is empty");
            setUnlocking(false);
            return;
        }
        refreshTokenReply = api->refreshAccessToken(tokenService->getClientIdFromToken(), refreshToken);
        connect(refreshTokenReply, &QNetworkReply::finished, this, &VaultManager::refreshTokenReplyFinished);
        return;
    }

    verifyPasswordOnline();
}

void VaultManager::lock()
{
    cryptoService->clearKey();
//    cryptoService->clearOrgKeys();
    cryptoService->clearKeyPair();
    cryptoService->clearEncKey();

    // TODO: clear view models
}

bool VaultManager::isLocked()
{
    return !cryptoService->hasKey();
}

void VaultManager::keyChanged()
{
    emit isLockedChanged();
}

bool VaultManager::getUnlocking() const
{
    return unlocking;
}

void VaultManager::setUnlocking(const bool &value)
{
    if(unlocking != value) {
        unlocking = value;
        emit unlockingChanged();
    }
}

QString VaultManager::getUnlockMessage() const
{
    return unlockMessage;
}

void VaultManager::refreshTokenReplyFinished()
{

    if(refreshTokenReply->error() != QNetworkReply::NoError && refreshTokenReply->error() != QNetworkReply::ProtocolInvalidOperationError){
        key.clear();
        masterPasswordHash.clear();
        setUnlockMessage("Token refreshing failed: " + refreshTokenReply->errorString());
        setUnlocking(false);
        return;
    }
    QJsonDocument json = QJsonDocument::fromJson(refreshTokenReply->readAll());
    QJsonObject root = json.object();
    if(refreshTokenReply->error() == QNetworkReply::ProtocolInvalidOperationError && root.contains("error")){
        key.clear();
        masterPasswordHash.clear();
        setUnlockMessage("Token refreshing failed. " + root["error"].toString());
        setUnlocking(false);
        return;
    }

    if(refreshTokenReply->error() == QNetworkReply::ProtocolInvalidOperationError){
        key.clear();
        masterPasswordHash.clear();
        setUnlockMessage(refreshTokenReply->errorString());
        setUnlocking(false);
        return;
    }

    tokenService->setTokens(root["access_token"].toString(), root["refresh_token"].toString());

    verifyPasswordOnline();
}

void VaultManager::verifyPasswordOnline()
{
    setUnlockMessage("Verify password online");
    verifyPasswordReply = api->postAccountVerifyPassword(masterPasswordHash, tokenService->getAccessToken());
    connect(verifyPasswordReply, &QNetworkReply::finished, this, &VaultManager::verifyPasswordFinished);
}

void VaultManager::verifyPasswordFinished()
{
//    QList<QPair<QByteArray, QByteArray>>::const_iterator i;
//    for (i = verifyPasswordReply->rawHeaderPairs().begin(); i != verifyPasswordReply->rawHeaderPairs().end(); ++i)
//        qDebug() << "header:" << (*i).first << ":" << (*i).second;
//    qDebug() << verifyPasswordReply->readAll();
    if(verifyPasswordReply->error() == QNetworkReply::NoError) {
        cryptoService->setKey(key);
        setUnlockMessage("");
    } else {
        setUnlockMessage("Invalid password");
    }
    key.clear();
    masterPasswordHash.clear();
    setUnlocking(false);
}

void VaultManager::setUnlockMessage(const QString &value)
{
    if(unlockMessage != value) {
        unlockMessage = value;
        emit unlockMessageChanged();
    }
}
