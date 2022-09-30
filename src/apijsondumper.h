#ifndef APIJSONDUMPER_H
#define APIJSONDUMPER_H

#include <QDebug>
#include <QJsonObject>
#include <QString>



class ApiJsonDumper
{
public:
    ApiJsonDumper();

    void dumpSyncFields(QJsonObject *object);
    void dumpProfileFields(QJsonObject *object);
    void dumpCipherFields(QJsonObject *object);
    void dumpCipherLoginFields(QJsonObject *object);
    void dumpCipherCardFields(QJsonObject *object);
    void dumpCipherIdentityFields(QJsonObject *object);
    void dumpCipherFieldFields(QJsonObject *object);
    void dumpCipherPasswordHistoryFields(QJsonObject *object);
    void dumpFolderFields(QJsonObject *object);

private:
    bool isSyncFieldsDumped = false;
    bool isProfileFieldsDumped = false;
    bool isCipherFieldsDumped = false;
    bool isCipherLoginFieldsDumped = false;
    bool isCipherCardFieldsDumped = false;
    bool isCipherIdentityFieldsDumped = false;
    bool isCipherFieldFieldsDumped = false;
    bool isCipherPasswordHistoryFieldsDumped = false;
    bool isFolderFieldsDumped = false;

    void dumpFields(QString name, QJsonObject *object);
};

#endif // APIJSONDUMPER_H
