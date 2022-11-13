#ifndef APITASK_H
#define APITASK_H

#include <QObject>

#include "src/api.h"

class ApiTask : public QObject
{
    Q_OBJECT
public:
    explicit ApiTask(Api *api, QObject *parent = nullptr);
    virtual void exec();
    virtual QNetworkReply *makeRequest() = 0;
    virtual void apiCallSuccess();
    virtual void apiCallFail(const QString& message);

protected:
    Api *api;
    QNetworkReply *reply;

signals:
    void success();
    void newStage(const QString& message);
    void fail(const QString& reason);

private slots:
    void requestFinish();

};

#endif // APITASK_H
