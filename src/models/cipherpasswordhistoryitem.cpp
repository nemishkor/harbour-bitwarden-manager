#include "cipherpasswordhistoryitem.h"

CipherPasswordHistoryItem::CipherPasswordHistoryItem(QString lastUsedDate, CipherString password):
    lastUsedDate(lastUsedDate),
    password(password)
{

}

QString CipherPasswordHistoryItem::getLastUsedDate() const
{
    return lastUsedDate;
}

void CipherPasswordHistoryItem::setLastUsedDate(const QString &value)
{
    lastUsedDate = value;
}

CipherString CipherPasswordHistoryItem::getPassword() const
{
    return password;
}

void CipherPasswordHistoryItem::fillPassword(const QString &value)
{
    password.fill(value);
}
