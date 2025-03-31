#include "Contact.h"
//���������
bool Contact::isValid() {
    QRegularExpression nameRegex("^[A-Za-z�-��-���]+[\\s-]?[A-Za-z�-��-���\\s-]*$");
    QRegularExpression phoneRegex("^\\+?\\d{1,3}\\s?\\(?\\d{1,4}?\\)?\\s?\\d{1,4}[-\\s]?\\d{1,4}[-\\s]?\\d{1,9}$");
    QRegularExpression emailRegex("^[A-Za-z0-9]+@[A-Za-z0-9]+\\.[A-Za-z]{2,}$");

    // �������� �����, ������� � ��������
    if (!nameRegex.match(firstName).hasMatch() ||
        !nameRegex.match(lastName).hasMatch() ||
        !nameRegex.match(middleName).hasMatch()) {
        return false;
    }

    // �������� ���� ��������
    if (birthDate >= QDate::currentDate()) {
        return false;
    }

    // �������� email
    if (!emailRegex.match(email).hasMatch()) {
        return false;
    }

    // �������� ���� ���������� �������
    for (const QString& phone : phoneNumbers) {
        if (!phoneRegex.match(phone).hasMatch()) {
            return false;
        }
    }

    return true;
}
