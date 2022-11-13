#ifndef FOLDER_H
#define FOLDER_H

#include "src/cipherstring.h"

#include <QString>

class Folder
{
public:
    Folder();
    QString getId() const;
    void setId(const QString &value);

    CipherString getName() const;
    void setName(const QString &value);

    QString getUserId() const;
    void setUserId(const QString &value);

    QString getRevisionDate() const;
    void setRevisionDate(const QString &value);

private:
    QString id;
    CipherString name;
    QString userId;
    QString revisionDate;
};

#endif // FOLDER_H
