#ifndef VAULT_H
#define VAULT_H

#include <QObject>

#include <src/services/stateservice.h>

class Vault : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int foldersCount READ foldersCount NOTIFY foldersCountChanged)
    Q_PROPERTY(int ciphersCount READ ciphersCount NOTIFY ciphersCountChanged)
    Q_PROPERTY(int ciphersDeletedCount READ ciphersDeletedCount NOTIFY ciphersCountDeletedChanged)
public:
    explicit Vault(StateService *stateService, QObject *parent = nullptr);

    int foldersCount();
    int ciphersCount();
    int ciphersDeletedCount();

private:
    StateService *stateService;
    QList<Cipher>::iterator cipherIt;

public slots:
    void foldersWereChanged();
    void ciphersWereChanged();

signals:
    void foldersCountChanged();
    void ciphersCountChanged();
    void ciphersCountDeletedChanged();

};

#endif // VAULT_H
