#ifndef IDENTITYCAPTCHARESPONSE_H
#define IDENTITYCAPTCHARESPONSE_H

#include <QString>

class IdentityCaptchaResponse
{
public:
    IdentityCaptchaResponse(QString key);
    QString getKey();
private:
    QString key;
};

#endif // IDENTITYCAPTCHARESPONSE_H
