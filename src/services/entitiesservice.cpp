#include "entitiesservice.h"

EntitiesService::EntitiesService(TasksListModel *tasksListModel,
                                 CipherService *cipherService,
                                 TokenService *tokenService,
                                 Api *api,
                                 QObject *parent)
    : QObject(parent),
      tasksListModel(tasksListModel),
      cipherService(cipherService),
      tokenService(tokenService),
      api(api)
{

}

void EntitiesService::removeSelectedCiphers()
{
    if(cipherService->getCiphersListModel()->getCheckedCount() == 0){
        qWarning() << "Unable to run 'remove' task. No one cipher is selected";
        return;
    }

    QModelIndexList removingCiphers = cipherService->getCiphersListModel()->match(
                cipherService->getCiphersListModel()->index(0, 0),
                CiphersListModel::CipherRoles::CheckStateRole,
                true,
                -1);

    if(removingCiphers.count() == 0){
        qWarning() << "Unable to run 'remove' task. No one cipher is selected 2";
        return;
    }

    QList<TaskListItem*> tasks;

    for(QModelIndex i : removingCiphers){
        // we can't add tasks to the model during index list iteration
        // task modifies the list
        if(!cipherService->getCiphersListModel()->data(i, CiphersListModel::RemovingRole).toBool()){
            QString cipherName = cipherService->getCiphersListModel()->data(i, CiphersListModel::NameRole).toString();
            QString cipherId = cipherService->getCiphersListModel()->data(i, CiphersListModel::IdRole).toString();
            RemoveCipherTask* apiTask = new RemoveCipherTask(cipherService, cipherId, tokenService, api);
            tasks.append(new TaskListItem("Remove cipher \"" + cipherName + "\"", apiTask, tasksListModel));
        }
    }

    for(TaskListItem* task : tasks){
        tasksListModel->add(task);
        task->start();
    }
}
