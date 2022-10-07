#ifndef FOLDERSERVICE_H
#define FOLDERSERVICE_H

#include <QDateTime>
#include <QObject>
#include <QList>
#include <QLocale>
#include <QTimeZone>

#include <src/factories/folderfactory.h>

#include <src/api.h>
#include <src/tokenservice.h>
#include <src/folder.h>
#include <src/models/folderslistmodel.h>
#include "src/cryptoservice.h"
#include "stateservice.h"

class FolderService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int creating READ getCreating NOTIFY creatingChanged)
public:
    FolderService(StateService *stateService, CryptoService *cryptoService,
                  TokenService *tokenService,
                  ApiJsonDumper *apiJsonDumper,
                  Api *api, QObject *parent = nullptr);

    Q_INVOKABLE void decryptAll();
    Q_INVOKABLE void create(QString name);

    bool getCreating();

    FoldersListModel *getListModel() const;
    void clear();

    void setSavingListItemFolder(FolderListItem *newSavingListItemFolder);

private:
    bool decrypted = false;
    bool creating = false;
    StateService *stateService;
    CryptoService *cryptoService;
    TokenService *tokenService;
    Api *api;
    FoldersListModel *listModel;
    QLocale locale;
    void setCreating(bool newCreating);
    FolderListItem *savingListItemFolder;
    QNetworkReply *postFolderReply;
    FolderFactory *folderFactory;
    void cancelSavingListItemFolder();

    void addListItemToModel(Folder *folder);

private slots:
    void postFolderRefreshTokenSuccess();
    void postFolderRefreshTokenFailed(QString reason);
    void postFolderSuccess();

signals:
    void creatingChanged();
};

#endif // FOLDERSERVICE_H
