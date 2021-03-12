#ifndef USER_H
#define USER_H

#include <QObject>
#include <QDebug>

#include "kdftype.h"

/**
 * Authorized user
 */
class User : public QObject
{
    Q_OBJECT
public:
    explicit User(QObject *parent = nullptr);
    void setInformation(QString userId, QString email, KdfType kdf, int kdfIterations);

private:
    QString userId;
    QString email;
    KdfType kdf;
    int kdfIterations;

signals:

};

#endif // USER_H
