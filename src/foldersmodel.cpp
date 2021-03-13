#include "foldersmodel.h"

FoldersModel::FoldersModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void FoldersModel::add(Folder &item)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    items << item;
    endInsertRows();
    emit countChanged();
}

void FoldersModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    items.clear();
    endRemoveRows();
    emit countChanged();
}

int FoldersModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return items.count();
}

QVariant FoldersModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= items.count())
        return QVariant();

    const Folder &folder = items[index.row()];
    if (role == IdRole)
        return folder.getId();
    if(role == NameRole)
        return folder.getName();
    if(role == UserIdRole)
        return folder.getUserId();
    if(role == RevisionDateRole)
        return folder.getRevisionDate();

    return QVariant();
}

QHash<int, QByteArray> FoldersModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    roles[UserIdRole] = "userId";
    roles[RevisionDateRole] = "revisionDate";

    return roles;
}
