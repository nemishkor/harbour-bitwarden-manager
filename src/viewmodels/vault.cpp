#include "vault.h"

Vault::Vault(StateService *stateService, QObject *parent) :
    QObject(parent),
    stateService(stateService)
{
    connect(stateService, &StateService::foldersWereChanged, this, &Vault::foldersWereChanged);
    connect(stateService, &StateService::ciphersWereChanged, this, &Vault::ciphersWereChanged);
}

int Vault::foldersCount()
{
    return stateService->getFolders()->count();
}

int Vault::ciphersCount()
{
    return stateService->getCiphers()->count();
}

int Vault::ciphersDeletedCount()
{
    QList<Cipher> *ciphers = stateService->getCiphers();
    int c = ciphers->count();
    for(cipherIt = ciphers->begin(); cipherIt != ciphers->end(); cipherIt++){
        if(cipherIt->getDeletedDate().isEmpty()){
            c--;
        }
    }
    return c;
}

void Vault::foldersWereChanged()
{
    emit foldersCountChanged();
}

void Vault::ciphersWereChanged()
{
    emit ciphersCountChanged();
    emit ciphersCountDeletedChanged();
}
