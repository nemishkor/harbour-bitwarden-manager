#ifndef DECRYPTEDCIPHER_H
#define DECRYPTEDCIPHER_H

#include <QString>

class CipherListItem
{
public:
    CipherListItem();

    QString getName() const;
    void setName(const QString &value);

    QString getLoginUsername() const;
    void setLoginUsername(const QString &value);

private:
    QString name;
    QString loginUsername;
};

#endif // DECRYPTEDCIPHER_H
