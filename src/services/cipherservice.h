#ifndef CIPHERSSERVICE_H
#define CIPHERSSERVICE_H

#include <QObject>
#include <QList>

#include "src/cipher.h"
#include "src/cipherfieldslistmodel.h"
#include "src/cipherlistitem.h"
#include "src/cipherview.h"
#include "src/cipherslistmodel.h"
#include "src/cipherpasswordhistoryitem.h"
#include "src/cipherpasswordhistorylistmodel.h"
#include "src/cryptoservice.h"

class CipherService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int count READ getCount NOTIFY countChanged)
    Q_PROPERTY(int countDeleted READ getCountDeleted NOTIFY countChanged)
public:
    explicit CipherService(CryptoService *cryptoService, CipherView *cipherView, QObject *parent = nullptr);

    Q_INVOKABLE void decryptAll(bool deletedOnly);
    Q_INVOKABLE void display(QString index);

    int getCount();
    int getCountDeleted();
    void add(Cipher &item);
    void clear();

    CiphersListModel *getCiphersListModel() const;

    CipherFieldsListModel *getCipherFieldsListModel() const;

    CipherPasswordHistoryListModel *getCipherPasswordHistoryListModel() const;

private:
    CryptoService *cryptoService;
    CipherView *cipherView;
    QList<Cipher> *ciphers; // all encrypted ciphers
    CiphersListModel *ciphersListModel; // filtered cached decrypted ciphers for list view
    CipherFieldsListModel *cipherFieldsListModel; // filtered cached decrypted fields of specific displayed cipher
    CipherPasswordHistoryListModel *cipherPasswordHistoryListModel; // filtered cached decrypted password history of specific displayed cipher

signals:
    void countChanged();

};

#endif // CIPHERSSERVICE_H
