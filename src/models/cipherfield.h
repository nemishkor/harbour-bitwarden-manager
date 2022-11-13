#ifndef CIPHERFIELD_H
#define CIPHERFIELD_H

#include "src/cipherstring.h"

class CipherField
{
public:
    enum FieldType {
        TEXT = 0,
        HIDDEN = 1,
        BOOLEAN = 2
    };
    CipherField(CipherString name, CipherField::FieldType type, CipherString value);

    CipherString getName() const;

    FieldType getType() const;

    CipherString getValue() const;

private:
    CipherString name;
    CipherField::FieldType type;
    CipherString value;

};

#endif // CIPHERFIELD_H
