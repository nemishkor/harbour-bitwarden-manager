#ifndef STATESERVICE_H
#define STATESERVICE_H

#include <QList>

#include "src/models/cipher.h"
#include "src/models/folder.h"

class StateService : public QObject
{
    Q_OBJECT
public:
    explicit StateService(QObject *parent = nullptr);
    ~StateService();

    void add(Folder &item);
    void add(Cipher &item);

    QList<Folder> *getFolders() const;
    void clear();

    QList<Cipher> *getCiphers() const;

private:
    QList<Folder> *folders;
    QList<Cipher> *ciphers;

signals:
    void foldersWereChanged();
    void ciphersWereChanged();

};

#endif // STATESERVICE_H
