#include "identitycaptcharesponse.h"

IdentityCaptchaResponse::IdentityCaptchaResponse(QString key) : key(key)
{

}

QString IdentityCaptchaResponse::getKey()
{
    return key;
}
