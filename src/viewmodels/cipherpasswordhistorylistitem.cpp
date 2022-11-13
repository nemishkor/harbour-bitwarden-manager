#include "cipherpasswordhistorylistitem.h"

CipherPasswordHistoryListItem::CipherPasswordHistoryListItem(const QString &lastUsedDate, const QString &password):
    lastUsedDate(lastUsedDate),
    password(password)
{

}

QString CipherPasswordHistoryListItem::getLastUsedDate() const
{
    return lastUsedDate;
}

QString CipherPasswordHistoryListItem::getPassword() const
{
    return password;
}
