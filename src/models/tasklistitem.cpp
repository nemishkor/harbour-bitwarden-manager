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

void TaskListItem::success(QString newMessage)
{
    message = newMessage;
    success();
}

void TaskListItem::fail()
{
    status = Enums::TaskStatus::Fail;
    emit updated(this);
}

void TaskListItem::fail(QString newMessage)
{
    message = newMessage;
    fail();
}

void TaskListItem::fail(QNetworkReply *failedReply)
{
    setMessage(getFailedReplyMessage(failedReply));
    fail();
}

void TaskListItem::fail(QString newMessage, QNetworkReply *failedReply)
{
    setMessage(newMessage + ". " + getFailedReplyMessage(failedReply));
    fail();
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

QString TaskListItem::getFailedReplyMessage(QNetworkReply *failedReply)
{
    return "API: [" + QString::number(failedReply->error()) + "]" + failedReply->errorString();
}

void TaskListItem::terminate()
{
    emit finished(this);
}
