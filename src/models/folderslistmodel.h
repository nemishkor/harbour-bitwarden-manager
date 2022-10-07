#ifndef FOLDERSLISTMODEL_H
#define FOLDERSLISTMODEL_H

#include "folderlistitem.h"

#include <QAbstractListModel>


class FoldersListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum FolderRoles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        RevisionDateRole
    };
    explicit FoldersListModel(QObject *parent = nullptr);

    void add(FolderListItem &item);
    void removeLast();
    void clear();
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;
private:
    QList<FolderListItem> items;

signals:
    void countChanged();
};

#endif // FOLDERSLISTMODEL_H
