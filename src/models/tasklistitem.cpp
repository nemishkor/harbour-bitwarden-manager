#include "tasklistitem.h"

TaskListItem::TaskListItem(QString name, QObject *parent):
    QObject(parent),
    name(name)
{
    status = Enums::TaskStatus::InProgress;
}

TaskListItem::TaskListItem(const TaskListItem &item):
    QObject(item.parent())
{
    name = item.name;
    status = item.status;
}

const QString &TaskListItem::getName() const
{
    return name;
}

Enums::TaskStatus TaskListItem::getStatus() const
{
    return status;
}

void TaskListItem::startOver()
{
    status = Enums::TaskStatus::InProgress;
    emit updated(this);
}

void TaskListItem::success()
{
    status = Enums::TaskStatus::Success;
    emit updated(this);
    QTimer::singleShot(pauseMsBeforeRemove, this, &TaskListItem::terminate);
}

void TaskListItem::fail()
{
    status = Enums::TaskStatus::Fail;
    emit updated(this);
}

const QString &TaskListItem::getMessage() const
{
    return message;
}

void TaskListItem::setMessage(const QString &newMessage)
{
    message = newMessage;
    emit updated(this);
}

void TaskListItem::terminate()
{
    emit finished(this);
}
