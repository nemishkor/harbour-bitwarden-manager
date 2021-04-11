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
public:
    explicit CipherView(QObject *parent = nullptr);

    QString getName() const;
    void setName(const QString &value);

    int getType() const;
    void setType(const Cipher::CipherType &value);

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

private:
    QString name;
    Cipher::CipherType type;
    QString notes;
    bool favorite;
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

signals:
    void nameChanged();
    void typeChanged();
    void notesChanged();
    void favoriteChanged();
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

};

#endif // CIPHERVIEW_H
