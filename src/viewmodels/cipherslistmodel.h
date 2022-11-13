#ifndef CIPHERSMODEL_H
#define CIPHERSMODEL_H

#include <QAbstractListModel>

#include "src/viewmodels/cipherlistitem.h"

class CiphersListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(int checkedCount READ getCheckedCount NOTIFY checkedCountChanged)

public:
    enum CipherRoles {
        CheckStateRole = Qt::CheckStateRole,
        NameRole = Qt::DisplayRole,
        IdRole = Qt::UserRole + 1,
        SubtitleRole,
        TypeRole,
        RemovingRole
    };
    explicit CiphersListModel(QObject *parent = nullptr);

    Q_INVOKABLE void toggleChecked(int index);
    Q_INVOKABLE void selectAll();
    Q_INVOKABLE void unselectAll();
    void add(const CipherListItem &item);
    void insert(const CipherListItem &item, int row);
    void clear();
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    int getCheckedCount();

protected:
    QHash<int, QByteArray> roleNames() const override;
private:
    QList<CipherListItem> items;
    int checkedCount = 0;

    void setCheckedCount(int newCheckedCount);
    void markAllIsCheckedValue(bool isChecked);

signals:
    void countChanged();
    void checkedCountChanged();
};

#endif // CIPHERSMODEL_H
