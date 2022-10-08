#ifndef TASKSLISTMODEL_H
#define TASKSLISTMODEL_H

#include "tasklistitem.h"

#include <QAbstractListModel>
#include <QDebug>
#include <QTimer>

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

    TaskListItem* add(TaskListItem &item);
    void remove(TaskListItem *item);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;
private:
    QList<TaskListItem> items;
    QModelIndex findByRef(TaskListItem *item);

signals:
    void countChanged();

private slots:
    void itemWasFinished(TaskListItem *item);
    void itemWasUpdated(TaskListItem *item);
};

#endif // TASKSLISTMODEL_H
