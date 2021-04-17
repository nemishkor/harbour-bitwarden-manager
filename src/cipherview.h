#ifndef CIPHERVIEW_H
#define CIPHERVIEW_H

#include <QObject>

#include "cipher.h"

class CipherView : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
    Q_PROPERTY(int type READ getType NOTIFY typeChanged)
    Q_PROPERTY(QString notes READ getNotes NOTIFY notesChanged)
    Q_PROPERTY(bool favorite READ getFavorite NOTIFY favoriteChanged)
    Q_PROPERTY(QString revisionDate READ getRevisionDate NOTIFY revisionDateChanged)
    Q_PROPERTY(QString loginUsername READ getLoginUsername NOTIFY loginUsernameChanged)
    Q_PROPERTY(QString loginPasswordRevisionDate READ getLoginPasswordRevisionDate NOTIFY loginPasswordRevisionDateChanged)
    Q_PROPERTY(QString loginPassword READ getLoginPassword NOTIFY loginPasswordChanged)
    Q_PROPERTY(QString loginUri READ getLoginUri NOTIFY loginUriChanged)
    Q_PROPERTY(QString loginTotp READ getLoginTotp NOTIFY loginTotpChanged)
    Q_PROPERTY(QString cardBrand READ getCardBrand NOTIFY cardBrandChanged)
    Q_PROPERTY(QString cardCardholder READ getCardCardholder NOTIFY cardCardholderChanged)
    Q_PROPERTY(QString cardCode READ getCardCode NOTIFY cardCodeChanged)
    Q_PROPERTY(QString cardExpMonth READ getCardExpMonth NOTIFY cardExpMonthChanged)
    Q_PROPERTY(QString cardExpYear READ getCardExpYear NOTIFY cardExpYearChanged)
    Q_PROPERTY(QString cardNumber READ getCardNumber NOTIFY cardNumberChanged)
    Q_PROPERTY(QString deletedDate READ getDeletedDate NOTIFY deletedDateChanged)
    Q_PROPERTY(QString identityAddress1 READ getIdentityAddress1 NOTIFY identityAddress1Changed)
    Q_PROPERTY(QString identityAddress2 READ getIdentityAddress2 NOTIFY identityAddress2Changed)
    Q_PROPERTY(QString identityAddress3 READ getIdentityAddress3 NOTIFY identityAddress3Changed)
    Q_PROPERTY(QString identityCity READ getIdentityCity NOTIFY identityCityChanged)
    Q_PROPERTY(QString identityCompany READ getIdentityCompany NOTIFY identityCompanyChanged)
    Q_PROPERTY(QString identityCountry READ getIdentityCountry NOTIFY identityCountryChanged)
    Q_PROPERTY(QString identityEmail READ getIdentityEmail NOTIFY identityEmailChanged)
    Q_PROPERTY(QString identityFirstName READ getIdentityFirstName NOTIFY identityFirstNameChanged)
    Q_PROPERTY(QString identityLastName READ getIdentityLastName NOTIFY identityLastNameChanged)
    Q_PROPERTY(QString identityLicenseNumber READ getIdentityLicenseNumber NOTIFY identityLicenseNumberChanged)
    Q_PROPERTY(QString identityMiddleName READ getIdentityMiddleName NOTIFY identityMiddleNameChanged)
    Q_PROPERTY(QString identityPassportNumber READ getIdentityPassportNumber NOTIFY identityPassportNumberChanged)
    Q_PROPERTY(QString identityPhone READ getIdentityPhone NOTIFY identityPhoneChanged)
    Q_PROPERTY(QString identityPostalCode READ getIdentityPostalCode NOTIFY identityPostalCodeChanged)
    Q_PROPERTY(QString identitySSN READ getIdentitySSN NOTIFY identitySSNChanged)
    Q_PROPERTY(QString identityState READ getIdentityState NOTIFY identityStateChanged)
    Q_PROPERTY(QString identityTitle READ getIdentityTitle NOTIFY identityTitleChanged)
    Q_PROPERTY(QString identityUsername READ getIdentityUsername NOTIFY identityUsernameChanged)
public:
    explicit CipherView(QObject *parent = nullptr);

    QString getName() const;
    void setName(const QString &value);

    int getType() const;
    void setType(const Cipher::CipherType &value);

    QString getRevisionDate() const;
    void setRevisionDate(const QString &value);

    QString getLoginUsername() const;
    void setLoginUsername(const QString &value);

    QString getLoginPasswordRevisionDate() const;
    void setLoginPasswordRevisionDate(const QString &value);

    QString getLoginPassword() const;
    void setLoginPassword(const QString &value);

    QString getLoginUri() const;
    void setLoginUri(const QString &value);

    QString getNotes() const;
    void setNotes(const QString &value);

    bool getFavorite() const;
    void setFavorite(bool value);

    QString getLoginTotp() const;
    void setLoginTotp(const QString &value);

    QString getCardBrand() const;
    void setCardBrand(const QString &value);

    QString getCardCardholder() const;
    void setCardCardholder(const QString &value);

    QString getCardCode() const;
    void setCardCode(const QString &value);

    QString getCardExpMonth() const;
    void setCardExpMonth(const QString &value);

    QString getCardExpYear() const;
    void setCardExpYear(const QString &value);

    QString getCardNumber() const;
    void setCardNumber(const QString &value);

    QString getDeletedDate() const;
    void setDeletedDate(const QString &value);

    QString getIdentityAddress1() const;
    void setIdentityAddress1(const QString &value);

    QString getIdentityAddress2() const;
    void setIdentityAddress2(const QString &value);

    QString getIdentityAddress3() const;
    void setIdentityAddress3(const QString &value);

    QString getIdentityCity() const;
    void setIdentityCity(const QString &value);

    QString getIdentityCompany() const;
    void setIdentityCompany(const QString &value);

    QString getIdentityCountry() const;
    void setIdentityCountry(const QString &value);

    QString getIdentityEmail() const;
    void setIdentityEmail(const QString &value);

    QString getIdentityFirstName() const;
    void setIdentityFirstName(const QString &value);

    QString getIdentityLastName() const;
    void setIdentityLastName(const QString &value);

    QString getIdentityLicenseNumber() const;
    void setIdentityLicenseNumber(const QString &value);

    QString getIdentityMiddleName() const;
    void setIdentityMiddleName(const QString &value);

    QString getIdentityPassportNumber() const;
    void setIdentityPassportNumber(const QString &value);

    QString getIdentityPhone() const;
    void setIdentityPhone(const QString &value);

    QString getIdentityPostalCode() const;
    void setIdentityPostalCode(const QString &value);

    QString getIdentitySSN() const;
    void setIdentitySSN(const QString &value);

    QString getIdentityState() const;
    void setIdentityState(const QString &value);

    QString getIdentityTitle() const;
    void setIdentityTitle(const QString &value);

    QString getIdentityUsername() const;
    void setIdentityUsername(const QString &value);

private:
    QString name;
    Cipher::CipherType type;
    QString notes;
    bool favorite;
    QString revisionDate;
    QString loginUsername;
    QString loginPasswordRevisionDate;
    QString loginPassword;
    QString loginUri;
    QString loginTotp;
    QString cardBrand;
    QString cardCardholder;
    QString cardCode;
    QString cardExpMonth;
    QString cardExpYear;
    QString cardNumber;
    QString deletedDate;
    QString identityAddress1;
    QString identityAddress2;
    QString identityAddress3;
    QString identityCity;
    QString identityCompany;
    QString identityCountry;
    QString identityEmail;
    QString identityFirstName;
    QString identityLastName;
    QString identityLicenseNumber;
    QString identityMiddleName;
    QString identityPassportNumber;
    QString identityPhone;
    QString identityPostalCode;
    QString identitySSN;
    QString identityState;
    QString identityTitle;
    QString identityUsername;

signals:
    void nameChanged();
    void typeChanged();
    void notesChanged();
    void favoriteChanged();
    void revisionDateChanged();
    void loginUsernameChanged();
    void loginPasswordRevisionDateChanged();
    void loginPasswordChanged();
    void loginUriChanged();
    void loginTotpChanged();
    void cardBrandChanged();
    void cardCardholderChanged();
    void cardCodeChanged();
    void cardExpMonthChanged();
    void cardExpYearChanged();
    void cardNumberChanged();
    void deletedDateChanged();
    void identityAddress1Changed();
    void identityAddress2Changed();
    void identityAddress3Changed();
    void identityCityChanged();
    void identityCompanyChanged();
    void identityCountryChanged();
    void identityEmailChanged();
    void identityFirstNameChanged();
    void identityLastNameChanged();
    void identityLicenseNumberChanged();
    void identityMiddleNameChanged();
    void identityPassportNumberChanged();
    void identityPhoneChanged();
    void identityPostalCodeChanged();
    void identitySSNChanged();
    void identityStateChanged();
    void identityTitleChanged();
    void identityUsernameChanged();

};

#endif // CIPHERVIEW_H
