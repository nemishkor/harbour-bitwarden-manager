#include "cipherslistmodel.h"

CiphersListModel::CiphersListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void CiphersListModel::toggleChecked(int index)
{
    items.at(index);
}

void CiphersListModel::selectAll()
{
    setCheckedCount(rowCount());
    markAllIsCheckedValue(true);
}

void CiphersListModel::unselectAll()
{
    setCheckedCount(0);
    markAllIsCheckedValue(false);
}

void CiphersListModel::add(const CipherListItem &item)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    items << item;
    endInsertRows();
    emit countChanged();
}

void CiphersListModel::insert(const CipherListItem &item, int row)
{
    if(row < 0 || row > rowCount()){
        qWarning() << "Row is out of range" << row;
        add(item);
        return;
    }
    beginInsertRows(QModelIndex(), row, row);
    items.insert(row, item);
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
    if(role == CheckStateRole)
        return item.getChecked();
    if(role == RemovingRole)
        return item.getRemoving();

    return QVariant();
}

bool CiphersListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() < 0 || index.row() >= items.count())
        return false;

    CipherListItem &item = items[index.row()];
    if(role == CheckStateRole){
        item.setChecked(value.toBool());
        setCheckedCount(checkedCount + (item.getChecked() ? 1 : -1));
        emit dataChanged(index, index, QVector<int>{role});
        return true;
    }
    if(role == RemovingRole){
        item.setRemoving(value.toBool());
        emit dataChanged(index, index, QVector<int>{role});
        return true;
    }

    return false;
}

bool CiphersListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if(row < 0 || row >= items.count()){
        qWarning() << "unable to remove row" << row;
        return false;
    }
    int last = row + count - 1;
    if(last < 0 || last >= items.count()){
        qWarning() << "unable to remove last row" << last;
        return false;
    }
    beginRemoveRows(parent, row, last);
    for(int i = 0; i < count; i++){
        CipherListItem &item = items[row];
        setCheckedCount(checkedCount + (item.getChecked() ? -1 : 0));
        items.removeAt(row);
    }
    endRemoveRows();
    return true;
}

Qt::ItemFlags CiphersListModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsUserCheckable;
}

int CiphersListModel::getCheckedCount()
{
    return checkedCount;
}

QHash<int, QByteArray> CiphersListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    roles[SubtitleRole] = "subtitle";
    roles[TypeRole] = "type";
    roles[CheckStateRole] = "checked";
    roles[RemovingRole] = "removing";
    return roles;
}

void CiphersListModel::setCheckedCount(int newCheckedCount)
{
    if(checkedCount != newCheckedCount){
        checkedCount = newCheckedCount;
        emit checkedCountChanged();
    }
}

void CiphersListModel::markAllIsCheckedValue(bool isChecked)
{
    int count = rowCount();

    if(count == 0){
        return;
    }

    for(int i = 0; i < count; i++){
        CipherListItem &item = items[i];
        item.setChecked(isChecked);
    }

    emit dataChanged(createIndex(0, 0), createIndex(count, 0), QVector<int>{CheckStateRole});
}
