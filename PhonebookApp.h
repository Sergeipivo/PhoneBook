#ifndef PHONEBOOKAPP_H
#define PHONEBOOKAPP_H

#include <QWidget>
#include <QList>
#include <QLineEdit>
#include <QDateEdit>
#include <QListWidget>
#include <QCoreApplication>
#include <QDebug>

#include "Contact.h"
#include "databasemanager.h"


class PhonebookApp : public QWidget {
    Q_OBJECT
private:
    DatabaseManager dbManager;
public:
    PhonebookApp(QWidget* parent = nullptr);


    QLineEdit* firstNameEdit;
    QLineEdit* lastNameEdit;
    QLineEdit* middleNameEdit;
    QLineEdit* addressEdit;
    QDateEdit* birthDateEdit;
    QLineEdit* emailEdit;
    QLineEdit* phoneEdit;
    QList<Contact> contacts;
    QListWidget* contactList;

    void loadContacts();
    void saveContacts();
    void addContact();
    void editContact();
    void deleteContact();
    void sortContacts();
    void searchContacts();

    /*PhonebookApp(const PhonebookApp&) = default;
    PhonebookApp& operator=(const PhonebookApp&) = default;*/
};

#endif // PHONEBOOKAPP_H