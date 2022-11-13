#include "ciphercard.h"

CipherCard::CipherCard()
{

}

CipherString CipherCard::getBrand() const
{
    return brand;
}

void CipherCard::fillBrand(const QString &value)
{
    brand.fill(value);
}

CipherString CipherCard::getCardholderName() const
{
    return cardholderName;
}

void CipherCard::fillCardholderName(const QString &value)
{
    cardholderName.fill(value);
}

CipherString CipherCard::getCode() const
{
    return code;
}

void CipherCard::fillCode(const QString &value)
{
    code.fill(value);
}

CipherString CipherCard::getExpMonth() const
{
    return expMonth;
}

void CipherCard::fillExpMonth(const QString &value)
{
    expMonth.fill(value);
}

CipherString CipherCard::getExpYear() const
{
    return expYear;
}

void CipherCard::fillExpYear(const QString &value)
{
    expYear.fill(value);
}

CipherString CipherCard::getNumber() const
{
    return number;
}

void CipherCard::fillNumber(const QString &value)
{
    number = value;
}
