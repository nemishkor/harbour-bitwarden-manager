#ifndef CIPHERSSERVICE_H
#define CIPHERSSERVICE_H

#include <QObject>
#include <QList>

#include "cryptoservice.h"
#include "stateservice.h"
#include "src/models/cipher.h"
#include "src/models/cipherfilters.h"
#include "src/viewmodels/cipherview.h"
#include "src/viewmodels/cipherfieldslistmodel.h"
#include "src/viewmodels/cipherlistitem.h"
#include "src/viewmodels/cipherpasswordhistorylistmodel.h"
#include "src/viewmodels/cipherslistmodel.h"

class CipherService : public QObject
{
    Q_OBJECT
public:
    explicit CipherService(StateService *stateService,
                           CryptoService *cryptoService,
                           CipherView *cipherView,
                           QObject *parent = nullptr);

    Q_INVOKABLE void decryptAll(bool deletedOnly, QString folderId);
    bool decryptOne(const QString id, const QModelIndex index);
    Q_INVOKABLE void display(QString index);
    void add(Cipher &item);
    void clear();
    QModelIndex removeFromView(QString id);
    bool removeFromState(QString id);
    bool restoreToView(QString id);

    CiphersListModel *getCiphersListModel() const;
    CipherFieldsListModel *getCipherFieldsListModel() const;
    CipherPasswordHistoryListModel *getCipherPasswordHistoryListModel() const;

private:
    // Deps
    StateService *stateService;
    CryptoService *cryptoService;
    CipherView *cipherView;

    CiphersListModel *ciphersListModel; // filtered cached decrypted ciphers for list view
    CipherFieldsListModel *cipherFieldsListModel; // filtered cached decrypted fields of specific displayed cipher
    CipherPasswordHistoryListModel *cipherPasswordHistoryListModel; // filtered cached decrypted password history of specific displayed cipher
    CipherFilters filters;

    CipherListItem decrypt(Cipher *cipher);

signals:
    void countChanged();

};

#endif // CIPHERSSERVICE_H
