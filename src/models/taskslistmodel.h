#ifndef TASKSLISTMODEL_H
#define TASKSLISTMODEL_H


#include <QAbstractListModel>
#include <QDebug>
#include <QTimer>

#include "tasklistitem.h"
#include "src/tasks/apitask.h"

class TasksListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum TaskRoles {
        NameRole = Qt::UserRole + 1,
        StatusRole,
        MessageRole
    };
    explicit TasksListModel(QObject *parent = nullptr);

    void add(TaskListItem *item);
    TaskListItem* create(QString name, ApiTask *apiTask = nullptr);
    void remove(TaskListItem *item);
    bool contains(TaskListItem *item);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;
private:
    QList<TaskListItem*> items;
    QModelIndex findByRef(TaskListItem *item);

signals:
    void countChanged();

private slots:
    void itemIsReadyToDestroy(TaskListItem *item);
    void itemWasUpdated(TaskListItem *item);
};

#endif // TASKSLISTMODEL_H
