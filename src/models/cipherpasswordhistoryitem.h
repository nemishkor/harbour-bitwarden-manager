#ifndef CIPHERPASSWORDHISTORYITEM_H
#define CIPHERPASSWORDHISTORYITEM_H

#include <QString>

#include "src/cipherstring.h"

class CipherPasswordHistoryItem
{
public:
    CipherPasswordHistoryItem(QString lastUsedDate, CipherString password);

    QString getLastUsedDate() const;
    void setLastUsedDate(const QString &value);

    CipherString getPassword() const;
    void fillPassword(const QString &value);

private:
    QString lastUsedDate;
    CipherString password;

};

#endif // CIPHERPASSWORDHISTORYITEM_H
