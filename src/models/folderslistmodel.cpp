#include "folderslistmodel.h"

FoldersListModel::FoldersListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void FoldersListModel::add(FolderListItem &item)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    items << item;
    endInsertRows();
    emit countChanged();
}

void FoldersListModel::removeLast()
{
    beginRemoveRows(QModelIndex(), rowCount() - 1, rowCount() - 1);
    items.removeLast();
    endRemoveRows();
    emit countChanged();
}

void FoldersListModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    items.clear();
    endRemoveRows();
    emit countChanged();
}

int FoldersListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return items.count();
}

QVariant FoldersListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= items.count())
        return QVariant();

    const FolderListItem &item = items[index.row()];
    if (role == IdRole)
        return item.getId();
    if (role == NameRole)
        return item.getName();
    if(role == RevisionDateRole)
        return item.getRevisionDate();

    return QVariant();
}

QHash<int, QByteArray> FoldersListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    roles[RevisionDateRole] = "revisionDate";
    return roles;
}
