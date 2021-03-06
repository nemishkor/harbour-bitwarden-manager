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
    if (role == IdRole)
        return item.getId();
    if (role == NameRole)
        return item.getName();
    if(role == SubtitleRole)
        return item.getSubtitle();
    if(role == TypeRole)
        return (int) item.getType();

    return QVariant();
}

QHash<int, QByteArray> CiphersListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    roles[SubtitleRole] = "subtitle";
    roles[TypeRole] = "type";
    return roles;
}
