#ifndef VAULT_H
#define VAULT_H

#include <QObject>

#include <src/services/stateservice.h>

class Vault : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int foldersCount READ foldersCount NOTIFY foldersCountChanged)
public:
    explicit Vault(StateService *stateService, QObject *parent = nullptr);

    int foldersCount();

private:
    StateService *stateService;

public slots:
    void foldersWereChanged();

signals:
    void foldersCountChanged();

};

#endif // VAULT_H
