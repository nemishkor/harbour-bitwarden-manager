#ifndef CIPHERPASSWORDHISTORYLISTMODEL_H
#define CIPHERPASSWORDHISTORYLISTMODEL_H

#include <QAbstractListModel>
#include <QList>

#include "cipherpasswordhistorylistitem.h"

class CipherPasswordHistoryListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum CipherPasswordHistoryRoles {
        LastUsedDateRole = Qt::UserRole + 1,
        PasswordRole
    };
    explicit CipherPasswordHistoryListModel(QObject *parent = nullptr);

    void add(CipherPasswordHistoryListItem item);
    void clear();
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;
private:
    QList<CipherPasswordHistoryListItem> items;

signals:
    void countChanged();

};

#endif // CIPHERPASSWORDHISTORYLISTMODEL_H
