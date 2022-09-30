#include "folderlistitem.h"

FolderListItem::FolderListItem()
{

}

const QString &FolderListItem::getId() const
{
    return id;
}

void FolderListItem::setId(const QString &newId)
{
    id = newId;
}

const QString &FolderListItem::getName() const
{
    return name;
}

void FolderListItem::setName(const QString &newName)
{
    name = newName;
}

const QString &FolderListItem::getRevisionDate() const
{
    return revisionDate;
}

void FolderListItem::setRevisionDate(const QString &newRevisionDate)
{
    revisionDate = newRevisionDate;
}
