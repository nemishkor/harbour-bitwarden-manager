#ifndef CIPHERFIELDSLISTMODEL_H
#define CIPHERFIELDSLISTMODEL_H

#include <QAbstractListModel>
#include <QList>

#include "cipherfieldlistitem.h"

class CipherFieldsListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum CipherFieldRoles {
        NameRole = Qt::UserRole + 1,
        TypeRole,
        ValueRole
    };
    explicit CipherFieldsListModel(QObject *parent = nullptr);

    void add(CipherFieldListItem item);
    void clear();
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;
private:
    QList<CipherFieldListItem> items;

signals:
    void countChanged();

};

#endif // CIPHERFIELDSLISTMODEL_H
