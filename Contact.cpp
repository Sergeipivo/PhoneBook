#include "Contact.h"
//валидатор
bool Contact::isValid() {
    QRegularExpression nameRegex("^[A-Za-zА-Яа-яЁё]+[\\s-]?[A-Za-zА-Яа-яЁё\\s-]*$");
    QRegularExpression phoneRegex("^\\+?\\d{1,3}\\s?\\(?\\d{1,4}?\\)?\\s?\\d{1,4}[-\\s]?\\d{1,4}[-\\s]?\\d{1,9}$");
    QRegularExpression emailRegex("^[A-Za-z0-9]+@[A-Za-z0-9]+\\.[A-Za-z]{2,}$");

    // Проверка имени, фамилии и отчества
    if (!nameRegex.match(firstName).hasMatch() ||
        !nameRegex.match(lastName).hasMatch() ||
        !nameRegex.match(middleName).hasMatch()) {
        return false;
    }

    // Проверка даты рождения
    if (birthDate >= QDate::currentDate()) {
        return false;
    }

    // Проверка email
    if (!emailRegex.match(email).hasMatch()) {
        return false;
    }

    // Проверка всех телефонных номеров
    for (const QString& phone : phoneNumbers) {
        if (!phoneRegex.match(phone).hasMatch()) {
            return false;
        }
    }

    return true;
}
