#ifndef APP_H
#define APP_H

#include <QObject>
#include <QQuickView>

#include "auth.h"
#include "vaultmanager.h"
#include "src/services/appidservice.h"
#include "src/services/entitiesservice.h"
#include "src/services/cipherservice.h"
#include "src/viewmodels/vault.h"

class App : public QObject
{
    Q_OBJECT
public:
    App(QQuickView& view, QObject* parent = nullptr);

    CryptoService *getCryptoService() const;

    EnvironmentService *getEnvironmentService() const;

    Api *getApi() const;

    TokenService *getTokenService() const;

    TasksListModel *getTasksListModel() const;

    CipherService *getCipherService() const;

    CipherView *getCipher() const;

    User *getUser() const;

    SyncService *getSyncService() const;

    Auth *getAuth() const;

    FolderService *getFoldersService() const;

    VaultManager *getVaultManager() const;

    Vault *getVault() const;

    EntitiesService *getEntitiesService() const;

private:
    CipherView *cipher;
    CiphersListModel *ciphersListModel;
    TasksListModel *tasksListModel;

    ApiJsonDumper *apiJsonDumper;

    CipherFactory *cipherFactory;
    FolderFactory *folderFactory;

    QSettings *settings;
    AppIdService *appIdService;
    User *user;
    StateService *stateService;
    EnvironmentService *environmentService;
    Api *api;
    TokenService *tokenService;
    CryptoService *cryptoService;
    Vault *vault;
    VaultManager *vaultManager;

    EntitiesService *entitiesService;
    CipherService *cipherService;
    FolderService *foldersService;
    SyncService *syncService;
    Auth *auth;

    QQuickView::Status viewStatus;

    void autoSync();

private slots:
    void setViewStatus(QQuickView::Status status);
};

#endif // APP_H
