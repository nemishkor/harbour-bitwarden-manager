#include "cipherfieldlistitem.h"

CipherFieldListItem::CipherFieldListItem(QString name, CipherField::FieldType type, QString value):
    name(name), type(type), value(value)
{

}

QString CipherFieldListItem::getName() const
{
    return name;
}

CipherField::FieldType CipherFieldListItem::getType() const
{
    return type;
}

QString CipherFieldListItem::getValue() const
{
    return value;
}
