#include "folderfactory.h"

FolderFactory::FolderFactory(ApiJsonDumper *apiJsonDumper, QObject* parent) :
    QObject(parent),
    apiJsonDumper(apiJsonDumper)
{

}

Folder FolderFactory::create(QJsonObject apiObject, QString userId)
{
    apiJsonDumper->dumpFolderFields(&apiObject);
    Folder folder;
    folder.setId(apiObject["id"].toString());
    folder.setName(apiObject["name"].toString());
    folder.setUserId(userId);
    folder.setRevisionDate(apiObject["revisionDate"].toString());

    return folder;
}
