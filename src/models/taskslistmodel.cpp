#include "taskslistmodel.h"

TasksListModel::TasksListModel(QObject *parent) : QAbstractListModel(parent)
{
}

TaskListItem* TasksListModel::add(TaskListItem &item)
{
    beginInsertRows(QModelIndex(), items.count(), items.count());
    items.append(item);
    TaskListItem *added = &items.last();
    connect(added, &TaskListItem::finished, this, &TasksListModel::itemWasFinished);
    connect(added, &TaskListItem::updated, this, &TasksListModel::itemWasUpdated);
    endInsertRows();
    emit countChanged();

    return added;
}

TaskListItem *TasksListModel::create(QString name, TaskListItem *oldTask)
{
    if(oldTask != nullptr){
        remove(oldTask);
    }
    TaskListItem task(name);
    return add(task);
}

int TasksListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return items.count();
}

QVariant TasksListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= items.count())
        return QVariant();

    const TaskListItem &item = items[index.row()];
    if (role == NameRole)
        return item.getName();
    if (role == StatusRole)
        return (int)item.getStatus();
    if (role == MessageRole)
        return item.getMessage();

    return QVariant();
}

QHash<int, QByteArray> TasksListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[StatusRole] = "status";
    roles[MessageRole] = "message";
    return roles;
}

QModelIndex TasksListModel::findByRef(TaskListItem *item)
{
    QList<TaskListItem>::const_iterator it;
    for(it = items.constBegin(); it != items.constEnd(); it++){
        if(&(*it) == item){
            int pos = it - items.begin();
            return createIndex(pos, 0);
        }
    }
    return createIndex(-1, 0);
}

void TasksListModel::itemWasFinished(TaskListItem *item)
{
    remove(item);
}

void TasksListModel::remove(TaskListItem *item)
{
    QModelIndex i = findByRef(item);
    if(i.isValid()){
        beginRemoveRows(QModelIndex(), i.row(), i.row());
        items.removeAt(i.row());
        endRemoveRows();
        emit countChanged();
        return;
    }
    qWarning() << "Unable to find item to remove. Probably it's removed already";
}

bool TasksListModel::contains(TaskListItem *item)
{
    return findByRef(item).isValid();
}

void TasksListModel::itemWasUpdated(TaskListItem *item)
{
    QModelIndex i = findByRef(item);
    emit dataChanged(i, i);
}
