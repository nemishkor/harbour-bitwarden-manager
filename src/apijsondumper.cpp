#include "apijsondumper.h"

ApiJsonDumper::ApiJsonDumper()
{

}

void ApiJsonDumper::dumpSyncFields(QJsonObject *object)
{
    if(isSyncFieldsDumped){
        return;
    }
    dumpFields("sync root", object);
    isSyncFieldsDumped = true;
}

void ApiJsonDumper::dumpProfileFields(QJsonObject *object)
{
    if(isProfileFieldsDumped){
        return;
    }
    dumpFields("profile", object);
    isProfileFieldsDumped = true;
}

void ApiJsonDumper::dumpCipherFields(QJsonObject *object)
{
    if(isCipherFieldsDumped){
        return;
    }
    dumpFields("cipher", object);
    isCipherFieldsDumped = true;
}

void ApiJsonDumper::dumpCipherLoginFields(QJsonObject *object)
{
    if(isCipherLoginFieldsDumped){
        return;
    }
    dumpFields("cipher['login']", object);
    isCipherLoginFieldsDumped = true;
}

void ApiJsonDumper::dumpCipherCardFields(QJsonObject *object)
{
    if(isCipherCardFieldsDumped){
        return;
    }
    dumpFields("cipher['card']", object);
    isCipherCardFieldsDumped = true;
}

void ApiJsonDumper::dumpCipherIdentityFields(QJsonObject *object)
{
    if(isCipherIdentityFieldsDumped){
        return;
    }
    dumpFields("cipher['identity']", object);
    isCipherIdentityFieldsDumped = true;
}

void ApiJsonDumper::dumpCipherFieldFields(QJsonObject *object)
{
    if(isCipherFieldFieldsDumped){
        return;
    }
    dumpFields("cipher['fields'][]", object);
    isCipherFieldFieldsDumped = true;
}

void ApiJsonDumper::dumpCipherPasswordHistoryFields(QJsonObject *object)
{
    if(isCipherPasswordHistoryFieldsDumped){
        return;
    }
    dumpFields("cipher['passwordHistory'][]", object);
    isCipherPasswordHistoryFieldsDumped = true;
}

void ApiJsonDumper::dumpFolderFields(QJsonObject *object)
{
    if(isFolderFieldsDumped){
        return;
    }
    dumpFields("['folder'][]", object);
    isFolderFieldsDumped = true;
}

void ApiJsonDumper::dumpFields(QString name, QJsonObject *object)
{
    qDebug() << "Start" << name << "object's fields list:";
    QJsonObject::const_iterator it;
    for(it = object->constBegin(); it != object->constEnd(); it++){
        qDebug() << it.key();
    }
    qDebug() << "End" << name << "object's fields list";
}
