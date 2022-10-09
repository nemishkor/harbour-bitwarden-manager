#ifndef FOLDERSERVICE_H
#define FOLDERSERVICE_H

#include <QDateTime>
#include <QObject>
#include <QList>
#include <QLocale>
#include <QTimeZone>

#include "src/api.h"
#include "src/cryptoservice.h"
#include "src/factories/folderfactory.h"
#include "src/folder.h"
#include "src/models/folderlistitem.h"
#include "src/models/folderslistmodel.h"
#include "src/models/taskslistmodel.h"
#include "src/tokenservice.h"
#include "stateservice.h"

class FolderService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int creating READ isCreating NOTIFY creatingChanged)
public:
    FolderService(StateService *stateService, CryptoService *cryptoService,
                  TokenService *tokenService,
                  ApiJsonDumper *apiJsonDumper,
                  TasksListModel *tasksListModel,
                  Api *api, QObject *parent = nullptr);

    Q_INVOKABLE void decryptAll();
    Q_INVOKABLE void create(QString name);

    bool isCreating();

    FoldersListModel *getListModel() const;
    void clear();

    void setSavingListItemFolder(FolderListItem *newSavingListItemFolder);

private:
    bool decrypted = false;
    StateService *stateService;
    CryptoService *cryptoService;
    TokenService *tokenService;
    Api *api;
    FoldersListModel *listModel;
    QLocale locale;
    FolderListItem *savingListItemFolder;
    QNetworkReply *postFolderReply;
    FolderFactory *folderFactory;
    TasksListModel *tasksListModel;
    TaskListItem *createFolderTask;
    void cancelSavingListItemFolder();

    void addListItemToModel(Folder *folder);

public slots:
    void createFolderTaskWasUpdated();

private slots:
    void postFolderRefreshTokenSuccess();
    void postFolderRefreshTokenFailed(QString reason);
    void postFolderSuccess();

signals:
    void creatingChanged();
};

#endif // FOLDERSERVICE_H
