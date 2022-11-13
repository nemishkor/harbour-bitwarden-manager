#include "app.h"

App::App(QQuickView& view, QObject* parent) : QObject(parent)
{
    cipher = new CipherView(this);
    tasksListModel = new TasksListModel(this);

    apiJsonDumper = new ApiJsonDumper(this);

    cipherFactory = new CipherFactory(apiJsonDumper, this);
    folderFactory = new FolderFactory(apiJsonDumper, this);

    stateService = new StateService(this);

    settings = new QSettings("nemishkor", "harbour-bitwarden-manager", this);
    appIdService = new AppIdService(settings, this);
    user = new User(settings, this);
    cryptoService = new CryptoService(settings, this);
    environmentService = new EnvironmentService(settings, this);
    api = new Api(settings, environmentService, this);
    tokenService = new TokenService(settings, api, this);
    vault = new Vault(stateService, this);
    vaultManager = new VaultManager(cryptoService, user, api, tokenService, this);

    cipherService = new CipherService(stateService, cryptoService, cipher, this);
    foldersService = new FolderService(folderFactory, stateService, cryptoService, tokenService, tasksListModel, api, this);
    entitiesService = new EntitiesService(tasksListModel, cipherService, tokenService, api, this);
    syncService = new SyncService(cipherFactory, folderFactory, api, user, tokenService, cryptoService, stateService, foldersService, cipherService, settings, apiJsonDumper, tasksListModel, this);
    auth = new Auth(appIdService, tokenService, api, cryptoService, user, syncService, tasksListModel, this);

    connect(&view, &QQuickView::statusChanged, this, &App::setViewStatus);
    connect(auth, &Auth::loginStageChanged, this, &App::autoSync);
    connect(syncService, &SyncService::synchronizedChanged, this, &App::autoSync);
}

CryptoService *App::getCryptoService() const
{
    return cryptoService;
}

EnvironmentService *App::getEnvironmentService() const
{
    return environmentService;
}

Api *App::getApi() const
{
    return api;
}

TokenService *App::getTokenService() const
{
    return tokenService;
}

TasksListModel *App::getTasksListModel() const
{
    return tasksListModel;
}

CipherService *App::getCipherService() const
{
    return cipherService;
}

CipherView *App::getCipher() const
{
    return cipher;
}

User *App::getUser() const
{
    return user;
}

SyncService *App::getSyncService() const
{
    return syncService;
}

Auth *App::getAuth() const
{
    return auth;
}

FolderService *App::getFoldersService() const
{
    return foldersService;
}

VaultManager *App::getVaultManager() const
{
    return vaultManager;
}

Vault *App::getVault() const
{
    return vault;
}

EntitiesService *App::getEntitiesService() const
{
    return entitiesService;
}

void App::autoSync()
{
    if(viewStatus == QQuickView::Status::Ready && auth->getLoginStage() == 2 && !syncService->isSynchronized()){
        syncService->syncAll();
    }
}

void App::setViewStatus(QQuickView::Status status)
{
    viewStatus = status;
    autoSync();
}
