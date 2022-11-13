#ifndef CIPHERFACTORY_H
#define CIPHERFACTORY_H

#include <QJsonObject>
#include <QJsonArray>

#include "src/apijsondumper.h"
#include "src/models/cipher.h"

class CipherFactory : QObject
{
    Q_OBJECT
public:
    CipherFactory(ApiJsonDumper *apiJsonDumper, QObject* parent = nullptr);
    Cipher create(QJsonObject apiObject, QString userId);

private:
    ApiJsonDumper *apiJsonDumper;
};

#endif // CIPHERFACTORY_H
