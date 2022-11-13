#ifndef CIPHERPASSWORDHISTORYLISTITEM_H
#define CIPHERPASSWORDHISTORYLISTITEM_H

#include <QString>

class CipherPasswordHistoryListItem
{
public:
    CipherPasswordHistoryListItem(const QString &lastUsedDate, const QString &password);

    QString getLastUsedDate() const;

    QString getPassword() const;

private:
    QString lastUsedDate;
    QString password;

};

#endif // CIPHERPASSWORDHISTORYLISTITEM_H
