#ifndef CIPHERSMODEL_H
#define CIPHERSMODEL_H

#include <QAbstractListModel>

#include "cipherlistitem.h"

class CiphersListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum CipherRoles {
        NameRole = Qt::UserRole + 1,
        LoginUsernameRole
    };
    explicit CiphersListModel(QObject *parent = nullptr);

    void add(CipherListItem &item);
    void clear();
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;
private:
    QList<CipherListItem> items;

signals:
    void countChanged();
};

#endif // CIPHERSMODEL_H
