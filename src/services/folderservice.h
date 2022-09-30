#ifndef FOLDERSERVICE_H
#define FOLDERSERVICE_H

#include <QDateTime>
#include <QObject>
#include <QList>
#include <QLocale>
#include <QTimeZone>

#include <src/folder.h>
#include <src/models/folderslistmodel.h>
#include "src/cryptoservice.h"
#include "stateservice.h"

class FolderService : public QObject
{
    Q_OBJECT
public:
    FolderService(StateService *stateService, CryptoService *cryptoService, QObject *parent = nullptr);

    Q_INVOKABLE void decryptAll();

    FoldersListModel *getListModel() const;
    void clear();

private:
    bool decrypted = false;
    StateService *stateService;
    CryptoService *cryptoService;
    FoldersListModel *listModel;
    QLocale locale;
};

#endif // FOLDERSERVICE_H
