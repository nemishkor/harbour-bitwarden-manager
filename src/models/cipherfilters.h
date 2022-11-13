#ifndef CIPHERFILTERS_H
#define CIPHERFILTERS_H

#include <QString>

#include "cipher.h"

class CipherFilters
{
public:
    CipherFilters();
    CipherFilters(bool deletedOnly, QString folderId);

    bool getDeletedOnly() const;

    const QString &getFolderId() const;

    bool filter(Cipher *cipher);

private:
    bool deletedOnly = false;
    QString folderId;
};

#endif // CIPHERFILTERS_H
