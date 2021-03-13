#ifndef CIPHERSMODEL_H
#define CIPHERSMODEL_H

#include <QAbstractListModel>

#include "cipher.h"

class CiphersModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum CipherRoles {
        IdRole = Qt::UserRole + 1,
        OrganizationIdRole,
        FolderIdRole,
        UserIdRole,
        EditRole,
        ViewPasswordRole,
        OrganizationUseTotpRole,
        FavoriteRole,
        RevisionDateRole,
        SizeNameRole,
        NameRole,
        NotesRole,
        DeletedDateRole
    };
    explicit CiphersModel(QObject *parent = nullptr);

    void add(Cipher &item);
    void clear();
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;
private:
    QList<Cipher> items;

signals:
    void countChanged();
};

#endif // CIPHERSMODEL_H
