#ifndef TASKLISTITEM_H
#define TASKLISTITEM_H

#include <QString>
#include <QTimer>
#include <QObject>
#include <QDebug>
#include <QNetworkReply>

#include <src/enums.h>



class TaskListItem : public QObject
{
    Q_OBJECT
public:
    TaskListItem(QString name, QObject *parent = nullptr);
    TaskListItem(const TaskListItem &item);

    const QString &getName() const;
    Enums::TaskStatus getStatus() const;
    void success();
    void success(QString newMessage);
    void fail();
    void fail(QString newMessage);
    void fail(QNetworkReply *failedReply);
    void fail(QString newMessage, QNetworkReply *failedReply);

    const QString &getMessage() const;
    void setMessage(const QString &newMessage);

private:
    QString name;
    Enums::TaskStatus status;
    QString message;
    const int pauseMsBeforeRemove = 4000;
    QString getFailedReplyMessage(QNetworkReply *failedReply);

signals:
    void finished(TaskListItem*);
    void updated(TaskListItem*);

private slots:
    void terminate();
};

#endif // TASKLISTITEM_H
