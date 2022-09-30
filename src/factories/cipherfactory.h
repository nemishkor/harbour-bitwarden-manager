#ifndef CIPHERFACTORY_H
#define CIPHERFACTORY_H

#include <src/apijsondumper.h>
#include <src/cipher.h>

#include <QJsonObject>
#include <QJsonArray>



class CipherFactory
{
public:
    CipherFactory(ApiJsonDumper *apiJsonDumper);
    Cipher create(QJsonObject apiObject, QString userId);

private:
    ApiJsonDumper *apiJsonDumper;
};

#endif // CIPHERFACTORY_H
