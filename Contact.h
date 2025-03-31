#ifndef CONTACT_H
#define CONTACT_H

#include <QString>
#include <QStringList>
#include <QDate>
#include <QRegularExpression>

class Contact {
public:
    QString firstName;
    QString lastName;
    QString middleName;
    QString address;
    QDate birthDate;
    QString email;
    QStringList phoneNumbers;

    bool isValid();
    Contact() {};
   
};

#endif // CONTACT_H