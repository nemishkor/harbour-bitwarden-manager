#ifndef CIPHERVIEW_H
#define CIPHERVIEW_H

#include <QObject>

#include "cipher.h"

class CipherView : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
    Q_PROPERTY(int type READ getType NOTIFY typeChanged)
    Q_PROPERTY(QString loginUsername READ getLoginUsername NOTIFY loginUsernameChanged)
public:
    explicit CipherView(QObject *parent = nullptr);

    QString getName() const;
    void setName(const QString &value);

    int getType() const;
    void setType(const Cipher::CipherType &value);

    QString getLoginUsername() const;
    void setLoginUsername(const QString &value);

private:
    QString name;
    Cipher::CipherType type;
    QString loginUsername;

signals:
    void nameChanged();
    void typeChanged();
    void loginUsernameChanged();

};

#endif // CIPHERVIEW_H
