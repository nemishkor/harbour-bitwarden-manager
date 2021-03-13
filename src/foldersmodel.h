#ifndef FOLDERSMODEL_H
#define FOLDERSMODEL_H

#include <QAbstractListModel>
#include <QList>

#include "folder.h"

class FoldersModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum FolderRoles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        UserIdRole,
        RevisionDateRole
    };

    explicit FoldersModel(QObject *parent = nullptr);

    void add(Folder &item);
    void clear();
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;
private:
    QList<Folder> items;

signals:
    void countChanged();
};

#endif // FOLDERSMODEL_H
