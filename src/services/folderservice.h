#ifndef FOLDERSERVICE_H
#define FOLDERSERVICE_H

#include <QDateTime>
#include <QObject>
#include <QList>
#include <QLocale>
#include <QTimeZone>

#include "cryptoservice.h"
#include "tokenservice.h"
#include "stateservice.h"
#include "src/api.h"
#include "src/factories/folderfactory.h"
#include "src/models/folder.h"
#include "src/models/folderlistitem.h"
#include "src/models/folderslistmodel.h"
#include "src/models/taskslistmodel.h"

class FolderService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool creating READ isCreating NOTIFY creatingChanged)
    Q_PROPERTY(bool removing READ isRemoving NOTIFY removingChanged)
public:
    FolderService(FolderFactory *folderService,
                  StateService *stateService,
                  CryptoService *cryptoService,
                  TokenService *tokenService,
                  TasksListModel *tasksListModel,
                  Api *api, QObject *parent = nullptr);

    Q_INVOKABLE void decryptAll();
    Q_INVOKABLE void create(QString name);
    Q_INVOKABLE void removeSelected();

    bool isCreating();
    bool isRemoving();

    FoldersListModel *getListModel() const;
    void clear();

    void setSavingListItemFolder(FolderListItem *newSavingListItemFolder);

private:
    bool decrypted = false;
    FolderFactory *folderFactory;
    StateService *stateService;
    CryptoService *cryptoService;
    TokenService *tokenService;
    Api *api;
    FoldersListModel *listModel;
    QLocale locale;
    FolderListItem *savingListItemFolder;
    QNetworkReply *postFolderReply;
    TasksListModel *tasksListModel;
    TaskListItem *createFolderTask;
    void cancelSavingListItemFolder();

    void addListItemToModel(Folder *folder);

    QStringList removingFolders;
    TaskListItem *removeFoldersTask;
    void setRemoveFoldersTask(TaskListItem *removeFoldersTask);

public slots:
    void createFolderTaskWasFinished();
    void createFolderTaskWasUpdated();
    void removeFolderTaskWasUpdated();

private slots:
    void postFolderRefreshTokenSuccess();
    void postFolderRefreshTokenFailed(QString reason);
    void postFolderSuccess();

signals:
    void creatingChanged();
    void removingChanged();
};

#endif // FOLDERSERVICE_H
