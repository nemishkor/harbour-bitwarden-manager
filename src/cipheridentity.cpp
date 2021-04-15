#include "cipheridentity.h"

CipherIdentity::CipherIdentity()
{

}

CipherString CipherIdentity::getAddress1() const
{
    return address1;
}

void CipherIdentity::fillAddress1(const QString &value)
{
    qDebug() << "fill address 1 before";
    address1.fill(value);
    qDebug() << "fill address 1 after";
}

CipherString CipherIdentity::getAddress2() const
{
    return address2;
}

void CipherIdentity::fillAddress2(const QString &value)
{
    address2.fill(value);
}

CipherString CipherIdentity::getAddress3() const
{
    return address3;
}

void CipherIdentity::fillAddress3(const QString &value)
{
    address3.fill(value);
}

CipherString CipherIdentity::getCity() const
{
    return city;
}

void CipherIdentity::fillCity(const QString &value)
{
    city.fill(value);
}

CipherString CipherIdentity::getCompany() const
{
    return company;
}

void CipherIdentity::fillCompany(const QString &value)
{
    company.fill(value);
}

CipherString CipherIdentity::getCountry() const
{
    return country;
}

void CipherIdentity::fillCountry(const QString &value)
{
    country.fill(value);
}

CipherString CipherIdentity::getEmail() const
{
    return email;
}

void CipherIdentity::fillEmail(const QString &value)
{
    email.fill(value);
}

CipherString CipherIdentity::getFirstName() const
{
    return firstName;
}

void CipherIdentity::fillFirstName(const QString &value)
{
    firstName.fill(value);
}

CipherString CipherIdentity::getLastName() const
{
    return lastName;
}

void CipherIdentity::fillLastName(const QString &value)
{
    lastName.fill(value);
}

CipherString CipherIdentity::getLicenseNumber() const
{
    return licenseNumber;
}

void CipherIdentity::fillLicenseNumber(const QString &value)
{
    licenseNumber.fill(value);
}

CipherString CipherIdentity::getMiddleName() const
{
    return middleName;
}

void CipherIdentity::fillMiddleName(const QString &value)
{
    middleName.fill(value);
}

CipherString CipherIdentity::getPassportNumber() const
{
    return passportNumber;
}

void CipherIdentity::fillPassportNumber(const QString &value)
{
    passportNumber.fill(value);
}

CipherString CipherIdentity::getPhone() const
{
    return phone;
}

void CipherIdentity::fillPhone(const QString &value)
{
    phone.fill(value);
}

CipherString CipherIdentity::getPostalCode() const
{
    return postalCode;
}

void CipherIdentity::fillPostalCode(const QString &value)
{
    postalCode.fill(value);
}

CipherString CipherIdentity::getSSN() const
{
    return SSN;
}

void CipherIdentity::fillSSN(const QString &value)
{
    SSN.fill(value);
}

CipherString CipherIdentity::getState() const
{
    return state;
}

void CipherIdentity::fillState(const QString &value)
{
    state.fill(value);
}

CipherString CipherIdentity::getTitle() const
{
    return title;
}

CipherString CipherIdentity::getUsername() const
{
    return username;
}

void CipherIdentity::fillTitle(const QString &value)
{
    title.fill(value);
}

void CipherIdentity::fillUsername(const QString &value)
{
    username.fill(value);
}
