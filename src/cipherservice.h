#ifndef CIPHERSSERVICE_H
#define CIPHERSSERVICE_H

#include <QObject>
#include <QList>

#include "cipher.h"
#include "cipherfieldslistmodel.h"
#include "cipherlistitem.h"
#include "cipherview.h"
#include "cipherslistmodel.h"
#include "cryptoservice.h"

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

private:
    CryptoService *cryptoService;
    CipherView *cipherView;
    QList<Cipher> *ciphers; // all encrypted ciphers
    CiphersListModel *ciphersListModel; // filtered cached decrypted ciphers for list view
    CipherFieldsListModel *cipherFieldsListModel; // filtered cached decrypted fields of specific displayed cipher

signals:
    void countChanged();

};

#endif // CIPHERSSERVICE_H
