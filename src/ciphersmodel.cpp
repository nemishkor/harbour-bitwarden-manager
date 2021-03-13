#include "ciphersmodel.h"

CiphersModel::CiphersModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void CiphersModel::add(Cipher &item)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    items << item;
    endInsertRows();
    emit countChanged();
}

void CiphersModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    items.clear();
    endRemoveRows();
    emit countChanged();
}

int CiphersModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return items.count();
}

QVariant CiphersModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= items.count())
        return QVariant();

    const Cipher &item = items[index.row()];
    if (role == IdRole)
        return item.getId();
    if(role == OrganizationIdRole)
        return item.getOrganizationId();
    if(role == FolderIdRole)
        return item.getFolderId();
    if(role == UserIdRole)
        return item.getUserId();
    if(role == EditRole)
        return item.getEdit();
    if(role == ViewPasswordRole)
        return item.getViewPassword();
    if(role == OrganizationUseTotpRole)
        return item.getOrganizationUseTotp();
    if(role == FavoriteRole)
        return item.getFavorite();
    if(role == RevisionDateRole)
        return item.getRevisionDate();
    if(role == SizeNameRole)
        return item.getSizeName();
    if(role == NameRole)
        return item.getName();
    if(role == NotesRole)
        return item.getNotes();
    if(role == DeletedDateRole)
        return item.getDeletedDate();

    return QVariant();
}

QHash<int, QByteArray> CiphersModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[OrganizationIdRole] = "organizationId";
    roles[FolderIdRole] = "folderId";
    roles[UserIdRole] = "userId";
    roles[EditRole] = "edit";
    roles[ViewPasswordRole] = "viewPassword";
    roles[OrganizationUseTotpRole] = "organizationUseTotp";
    roles[FavoriteRole] = "favorite";
    roles[RevisionDateRole] = "revisionDate";
    roles[SizeNameRole] = "sizeName";
    roles[NameRole] = "name";
    roles[NotesRole] = "notes";
    roles[DeletedDateRole] = "deletedDate";

    return roles;
}
