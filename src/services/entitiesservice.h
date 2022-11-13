#ifndef ENTITIESSERVICE_H
#define ENTITIESSERVICE_H

#include <QObject>

#include "cipherservice.h"
#include "tokenservice.h"
#include "src/tasks/removeciphertask.h"
#include "src/models/tasklistitem.h"
#include "src/models/taskslistmodel.h"

class EntitiesService : public QObject
{
    Q_OBJECT
public:
    explicit EntitiesService(TasksListModel *tasksListModel,
                             CipherService *cipherService,
                             TokenService *tokenService,
                             Api *api,
                             QObject *parent = nullptr);
    Q_INVOKABLE void removeSelectedCiphers();

private:
    TasksListModel *tasksListModel;
    CipherService *cipherService;
    TokenService *tokenService;
    Api *api;

};

#endif // ENTITIESSERVICE_H
