#ifndef STATESERVICE_H
#define STATESERVICE_H

#include <QList>

#include <src/folder.h>

class StateService : public QObject
{
    Q_OBJECT
public:
    explicit StateService(QObject *parent = nullptr);

    void add(Folder &item);

    QList<Folder> *getFolders() const;
    void clear();

private:
    QList<Folder> *folders;

signals:
    void foldersWereChanged();

};

#endif // STATESERVICE_H
