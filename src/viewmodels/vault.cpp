#include "vault.h"

Vault::Vault(StateService *stateService, QObject *parent) :
    QObject(parent),
    stateService(stateService)
{
    connect(stateService, &StateService::foldersWereChanged, this, &Vault::foldersWereChanged);
}

int Vault::foldersCount()
{
    return stateService->getFolders()->count();
}

void Vault::foldersWereChanged()
{
    emit foldersCountChanged();
}
