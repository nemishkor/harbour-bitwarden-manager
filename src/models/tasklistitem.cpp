#include "tasklistitem.h"

TaskListItem::TaskListItem(QString name, ApiTask *apiTask, QObject *parent):
    QObject(parent),
    apiTask(apiTask),
    name(name)
{
    if(apiTask != nullptr){
        apiTask->setParent(this);
    }
    status = Enums::TaskStatus::Waiting;
}

TaskListItem::~TaskListItem()
{
    if(apiTask != nullptr){
        delete apiTask;
    }
}

void TaskListItem::start()
{
    status = Enums::TaskStatus::InProgress;
    if(apiTask != nullptr){
        connect(apiTask, &ApiTask::success, this, &TaskListItem::apiTaskSuccessfullyFinished);
        connect(apiTask, &ApiTask::fail, this, &TaskListItem::apiTaskFailed);
        connect(apiTask, &ApiTask::newStage, this, &TaskListItem::apiTaskNewStage);
        qDebug() << "starting apiTask";
        apiTask->exec();
    }
}

const QString &TaskListItem::getName() const
{
    return name;
}

Enums::TaskStatus TaskListItem::getStatus() const
{
    return status;
}

void TaskListItem::success()
{
    status = Enums::TaskStatus::Success;
    qDebug().nospace().noquote() << "Task \"" << name << "\" successfuly finished";
    emit updated(this);
    emit finished(this);
    QTimer::singleShot(pauseMsBeforeRemove, this, &TaskListItem::terminate);
}

void TaskListItem::success(QString newMessage)
{
    message = newMessage;
    qDebug().nospace().noquote() << "Task \"" << name << "\" successfuly finished: " << message;
    success();
}

void TaskListItem::fail()
{
    status = Enums::TaskStatus::Fail;
    qWarning().nospace().noquote() << "Task \"" << name << "\" failed";
    emit updated(this);
    emit finished(this);
}

void TaskListItem::fail(QString newMessage)
{
    message = newMessage;
    qWarning().nospace().noquote() << "Task \"" << name << "\" failed: " << message;
    fail();
}

const QString &TaskListItem::getMessage() const
{
    return message;
}

void TaskListItem::setMessage(const QString &newMessage)
{
    message = newMessage;
    qDebug().nospace().noquote() << "Task \"" << name << "\": " << message;
    emit updated(this);
}

void TaskListItem::setStatus(Enums::TaskStatus newStatus)
{
    status = newStatus;
    emit updated(this);
}

void TaskListItem::terminate()
{
    emit readyToDestroy(this);
}

void TaskListItem::apiTaskSuccessfullyFinished()
{
    success();
}

void TaskListItem::apiTaskFailed(const QString& reason)
{
    fail(reason);
}

void TaskListItem::apiTaskNewStage(const QString &message)
{
    setMessage(message);
}
