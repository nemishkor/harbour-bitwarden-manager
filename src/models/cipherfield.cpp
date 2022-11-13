#include "cipherfield.h"

CipherField::CipherField(CipherString name, CipherField::FieldType type, CipherString value):
    name(name), type(type), value(value)
{

}

CipherString CipherField::getName() const
{
    return name;
}

CipherField::FieldType CipherField::getType() const
{
    return type;
}

CipherString CipherField::getValue() const
{
    return value;
}
