#include "cipherslistmodel.h"

CiphersListModel::CiphersListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void CiphersListModel::add(CipherListItem &item)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    items << item;
    endInsertRows();
    emit countChanged();
}

void CiphersListModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    items.clear();
    endRemoveRows();
    emit countChanged();
}

int CiphersListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return items.count();
}

QVariant CiphersListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= items.count())
        return QVariant();

    const CipherListItem &item = items[index.row()];
    if (role == NameRole)
        return item.getName();
    if(role == LoginUsernameRole)
        return item.getLoginUsername();

    return QVariant();
}

QHash<int, QByteArray> CiphersListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[LoginUsernameRole] = "loginUsername";

    return roles;
}
