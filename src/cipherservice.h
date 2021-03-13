#ifndef CIPHERSERVICE_H
#define CIPHERSERVICE_H

#include <QObject>

class CipherService : public QObject
{
    Q_OBJECT
public:
    explicit CipherService(QObject *parent = nullptr);

signals:

};

#endif // CIPHERSERVICE_H
