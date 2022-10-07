#include "folderfactory.h"

FolderFactory::FolderFactory(ApiJsonDumper *apiJsonDumper):
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
