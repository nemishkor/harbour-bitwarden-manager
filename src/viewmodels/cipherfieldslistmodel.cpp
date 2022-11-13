#include "cipherfieldslistmodel.h"

CipherFieldsListModel::CipherFieldsListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void CipherFieldsListModel::add(CipherFieldListItem item)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    items << item;
    endInsertRows();
    emit countChanged();
}

void CipherFieldsListModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    items.clear();
    endRemoveRows();
    emit countChanged();
}

int CipherFieldsListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return items.count();
}

QVariant CipherFieldsListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= items.count())
        return QVariant();

    const CipherFieldListItem &item = items[index.row()];
    if (role == NameRole)
        return item.getName();
    if(role == TypeRole)
        return (int) item.getType();
    if(role == ValueRole)
        return item.getValue();

    return QVariant();
}

QHash<int, QByteArray> CipherFieldsListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[TypeRole] = "type";
    roles[ValueRole] = "value";
    return roles;
}
