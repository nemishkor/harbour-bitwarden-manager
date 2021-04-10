#ifndef CIPHERFIELDLISTITEM_H
#define CIPHERFIELDLISTITEM_H

#include <QString>

#include "cipherfield.h"

class CipherFieldListItem
{
public:
    CipherFieldListItem(QString name, CipherField::FieldType type, QString value);

    QString getName() const;

    CipherField::FieldType getType() const;

    QString getValue() const;

private:
    QString name;
    CipherField::FieldType type;
    QString value;

};

#endif // CIPHERFIELDLISTITEM_H
