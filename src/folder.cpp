#include "folder.h"

Folder::Folder()
{

}

QString Folder::getId() const
{
    return id;
}

void Folder::setId(const QString &value)
{
    id = value;
}

CipherString Folder::getName() const
{
    return name;
}

void Folder::setName(const QString &value)
{
    name.fill(value);
}

QString Folder::getUserId() const
{
    return userId;
}

void Folder::setUserId(const QString &value)
{
    userId = value;
}

QString Folder::getRevisionDate() const
{
    return revisionDate;
}

void Folder::setRevisionDate(const QString &value)
{
    revisionDate = value;
}
