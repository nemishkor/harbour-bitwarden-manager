#ifndef FOLDERFACTORY_H
#define FOLDERFACTORY_H

#include <QJsonObject>

#include "src/apijsondumper.h"
#include "src/models/folder.h"

class FolderFactory : QObject
{
    Q_OBJECT
public:
    FolderFactory(ApiJsonDumper *apiJsonDumper, QObject* parent = nullptr);
    Folder create(QJsonObject apiObject, QString userId);

private:
    ApiJsonDumper *apiJsonDumper;
};

#endif // FOLDERFACTORY_H
