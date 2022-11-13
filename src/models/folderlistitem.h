#ifndef FOLDERLISTITEM_H
#define FOLDERLISTITEM_H

#include <QString>

class FolderListItem
{
public:
    FolderListItem();

    const QString &getId() const;
    void setId(const QString &newId);

    const QString &getName() const;
    void setName(const QString &newName);

    const QString &getRevisionDate() const;
    void setRevisionDate(const QString &newRevisionDate);

private:
    QString id;
    QString name;
    QString revisionDate;
};

#endif // FOLDERLISTITEM_H
