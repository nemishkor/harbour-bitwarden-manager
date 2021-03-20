#ifndef CIPHERSSERVICE_H
#define CIPHERSSERVICE_H

#include <QObject>
#include <QList>

#include "cipher.h"
#include "cipherlistitem.h"
#include "cipherslistmodel.h"
#include "cryptoservice.h"

class CipherService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int count READ getCount NOTIFY countChanged)
public:
    explicit CipherService(CryptoService *cryptoService, QObject *parent = nullptr);

    Q_INVOKABLE void decryptAll();

    int getCount();
    void add(Cipher &item);
    void clear();

    CiphersListModel *getCiphersListModel() const;

private:
    CryptoService *cryptoService;
    QList<Cipher> *ciphers; // all encrypted ciphers
    CiphersListModel *ciphersListModel; // filtered cached decrypted ciphers for list view

signals:
    void countChanged();

};

#endif // CIPHERSSERVICE_H
