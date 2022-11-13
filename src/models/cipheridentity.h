#ifndef CIPHERIDENTITY_H
#define CIPHERIDENTITY_H

#include <QDebug>
#include <QString>

#include "src/cipherstring.h"

class CipherIdentity
{
public:
    CipherIdentity();
    CipherIdentity(const CipherIdentity& identity):
        address1(identity.address1),
        address2(identity.address2),
        address3(identity.address3),
        city(identity.city),
        company(identity.company),
        country(identity.country),
        email(identity.email),
        firstName(identity.firstName),
        lastName(identity.lastName),
        licenseNumber(identity.licenseNumber),
        middleName(identity.middleName),
        passportNumber(identity.passportNumber),
        phone(identity.phone),
        postalCode(identity.postalCode),
        SSN(identity.SSN),
        state(identity.state),
        title(identity.title),
        username(identity.username)
    {
    }

    CipherString getAddress1() const;
    void fillAddress1(const QString &value);

    CipherString getAddress2() const;
    void fillAddress2(const QString &value);

    CipherString getAddress3() const;
    void fillAddress3(const QString &value);

    CipherString getCity() const;
    void fillCity(const QString &value);

    CipherString getCompany() const;
    void fillCompany(const QString &value);

    CipherString getCountry() const;
    void fillCountry(const QString &value);

    CipherString getEmail() const;
    void fillEmail(const QString &value);

    CipherString getFirstName() const;
    void fillFirstName(const QString &value);

    CipherString getLastName() const;
    void fillLastName(const QString &value);

    CipherString getLicenseNumber() const;
    void fillLicenseNumber(const QString &value);

    CipherString getMiddleName() const;
    void fillMiddleName(const QString &value);

    CipherString getPassportNumber() const;
    void fillPassportNumber(const QString &value);

    CipherString getPhone() const;
    void fillPhone(const QString &value);

    CipherString getPostalCode() const;
    void fillPostalCode(const QString &value);

    CipherString getSSN() const;
    void fillSSN(const QString &value);

    CipherString getState() const;
    void fillState(const QString &value);

    CipherString getTitle() const;
    void fillTitle(const QString &value);

    CipherString getUsername() const;
    void fillUsername(const QString &value);

private:
    CipherString address1;
    CipherString address2;
    CipherString address3;
    CipherString city;
    CipherString company;
    CipherString country;
    CipherString email;
    CipherString firstName;
    CipherString lastName;
    CipherString licenseNumber;
    CipherString middleName;
    CipherString passportNumber;
    CipherString phone;
    CipherString postalCode;
    CipherString SSN;
    CipherString state;
    CipherString title;
    CipherString username;

};

#endif // CIPHERIDENTITY_H
