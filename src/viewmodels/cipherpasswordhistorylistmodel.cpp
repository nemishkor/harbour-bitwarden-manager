#include "cipherpasswordhistorylistmodel.h"

CipherPasswordHistoryListModel::CipherPasswordHistoryListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void CipherPasswordHistoryListModel::add(CipherPasswordHistoryListItem item)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    items << item;
    endInsertRows();
    emit countChanged();
}

void CipherPasswordHistoryListModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    items.clear();
    endRemoveRows();
    emit countChanged();
}

int CipherPasswordHistoryListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return items.count();
}

QVariant CipherPasswordHistoryListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= items.count())
        return QVariant();

    const CipherPasswordHistoryListItem &item = items[index.row()];
    if (role == LastUsedDateRole)
        return item.getLastUsedDate();
    if(role == PasswordRole)
        return item.getPassword();

    return QVariant();
}

QHash<int, QByteArray> CipherPasswordHistoryListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[LastUsedDateRole] = "lastUsedDate";
    roles[PasswordRole] = "password";
    return roles;
}
