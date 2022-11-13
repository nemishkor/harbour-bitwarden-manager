#ifndef TASKLISTITEM_H
#define TASKLISTITEM_H

#include <QString>
#include <QTimer>
#include <QObject>
#include <QDebug>

#include "src/api.h"
#include "src/enums.h"
#include "src/tasks/apitask.h"

class TaskListItem : public QObject
{
    Q_OBJECT
public:
    TaskListItem(QString name, ApiTask *apiTask = nullptr, QObject *parent = nullptr);
    ~TaskListItem();

    void start();
    const QString &getName() const;
    Enums::TaskStatus getStatus() const;
    void success();
    void success(QString newMessage);
    void fail();
    void fail(QString newMessage);

    const QString &getMessage() const;
    void setMessage(const QString &newMessage);

protected:
    void setStatus(Enums::TaskStatus newStatus);

private:
    ApiTask *apiTask = nullptr;
    QString name;
    Enums::TaskStatus status;
    QString message;
    const int pauseMsBeforeRemove = 4000;

signals:
    void readyToDestroy(TaskListItem*);
    void finished(TaskListItem*);
    void updated(TaskListItem*);

private slots:
    void terminate();
    void apiTaskSuccessfullyFinished();
    void apiTaskFailed(const QString& reason);
    void apiTaskNewStage(const QString& reason);
};

#endif // TASKLISTITEM_H
