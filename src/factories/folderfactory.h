#ifndef FOLDERFACTORY_H
#define FOLDERFACTORY_H

#include <src/apijsondumper.h>
#include <src/folder.h>

#include <QJsonObject>

class FolderFactory
{
public:
    FolderFactory(ApiJsonDumper *apiJsonDumper);
    Folder create(QJsonObject apiObject, QString userId);

private:
    ApiJsonDumper *apiJsonDumper;
};

#endif // FOLDERFACTORY_H
