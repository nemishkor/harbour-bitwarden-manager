#ifndef FOLDER_H
#define FOLDER_H

#include <QString>

class Folder
{
public:
    Folder();
    QString getId() const;
    void setId(const QString &value);

    QString getName() const;
    void setName(const QString &value);

    QString getUserId() const;
    void setUserId(const QString &value);

    QString getRevisionDate() const;
    void setRevisionDate(const QString &value);

private:
    QString id;
    QString name;
    QString userId;
    QString revisionDate;
};

#endif // FOLDER_H
