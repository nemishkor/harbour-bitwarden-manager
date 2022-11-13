#ifndef CIPHERCARD_H
#define CIPHERCARD_H

#include <QDebug>
#include <QString>

#include "src/cipherstring.h"


class CipherCard
{
public:
    CipherCard();
    CipherCard(const CipherCard& card) :
        brand(card.brand),
        cardholderName(card.cardholderName),
        code(card.code),
        expMonth(card.expMonth),
        expYear(card.expYear),
        number(card.number)
    {}

    CipherString getBrand() const;
    void fillBrand(const QString &value);

    CipherString getCardholderName() const;
    void fillCardholderName(const QString &value);

    CipherString getCode() const;
    void fillCode(const QString &value);

    CipherString getExpMonth() const;
    void fillExpMonth(const QString &value);

    CipherString getExpYear() const;
    void fillExpYear(const QString &value);

    CipherString getNumber() const;
    void fillNumber(const QString &value);

private:
    CipherString brand;
    CipherString cardholderName;
    CipherString code;
    CipherString expMonth;
    CipherString expYear;
    CipherString number;

};

#endif // CIPHERCARD_H
