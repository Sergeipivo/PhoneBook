#include "PhonebookApp.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>
#include <QFile>
#include <QTextStream>
#include <algorithm>
#include <qlabel.h>
#include <QComboBox>



//#include <qgridlayout.h>

PhonebookApp::PhonebookApp(QWidget* parent) : QWidget(parent), dbManager("contacts.db") {
    QVBoxLayout* layout = new QVBoxLayout(this);
  
    contactList = new QListWidget(this);

    QPushButton* addButton = new QPushButton("Add", this);
    QPushButton* editButton = new QPushButton("Edit", this);
    QPushButton* deleteButton = new QPushButton("Delete", this);
    QPushButton* sortButton = new QPushButton("Sort", this);
    QPushButton* searchButton = new QPushButton("Search", this);

    // Добавление меток для каждого поля ввода
    layout->addWidget(new QLabel("Name:", this));
    firstNameEdit = new QLineEdit(this);
    layout->addWidget(firstNameEdit);

    layout->addWidget(new QLabel("Last name:", this));
    lastNameEdit = new QLineEdit(this);
    layout->addWidget(lastNameEdit);

    layout->addWidget(new QLabel("Middle name:", this));
    middleNameEdit = new QLineEdit(this);
    layout->addWidget(middleNameEdit);

    layout->addWidget(new QLabel("Addres:", this));
    addressEdit = new QLineEdit(this);
    layout->addWidget(addressEdit);

    layout->addWidget(new QLabel("Birth date:", this));
    birthDateEdit = new QDateEdit(this);
    layout->addWidget(birthDateEdit);

    layout->addWidget(new QLabel("Email:", this));
    emailEdit = new QLineEdit(this);
    layout->addWidget(emailEdit);

    layout->addWidget(new QLabel("Phone numbers:", this));
    phoneEdit = new QLineEdit(this);
    layout->addWidget(phoneEdit);

    


    

    
    layout->addWidget(addButton);
    layout->addWidget(editButton);
    layout->addWidget(deleteButton);
    layout->addWidget(sortButton);
    layout->addWidget(searchButton);
    layout->addWidget(contactList);

    connect(addButton, &QPushButton::clicked, this, &PhonebookApp::addContact);
    connect(editButton, &QPushButton::clicked, this, &PhonebookApp::editContact);
    connect(deleteButton, &QPushButton::clicked, this, &PhonebookApp::deleteContact);
    connect(sortButton, &QPushButton::clicked, this, &PhonebookApp::sortContacts);
    connect(searchButton, &QPushButton::clicked, this, &PhonebookApp::searchContacts);

    contacts = dbManager.getAllContacts();
    //loadContacts();

}



void PhonebookApp::addContact() {
    Contact newContact;
    newContact.firstName = firstNameEdit->text().trimmed();
    newContact.lastName = lastNameEdit->text().trimmed();
    newContact.middleName = middleNameEdit->text().trimmed();
    newContact.address = addressEdit->text().trimmed();
    newContact.birthDate = birthDateEdit->date();
    newContact.email = emailEdit->text().trimmed();
    newContact.phoneNumbers.append(phoneEdit->text().trimmed());

    if (newContact.isValid()) {
        dbManager.addContact(newContact);
        contacts.append(newContact);
        contactList->addItem(newContact.firstName + " " + newContact.lastName);
        saveContacts();
    }
    else {
        QMessageBox::warning(this, "Error", "Incorrect data!");
    }
}



void PhonebookApp::editContact() {
    int currentRow = contactList->currentRow();
    if (currentRow < 0) return;

    Contact& contact = contacts[currentRow];
    contact.firstName = firstNameEdit->text().trimmed();
    contact.lastName = lastNameEdit->text().trimmed();
    contact.middleName = middleNameEdit->text().trimmed();
    contact.address = addressEdit->text().trimmed();
    contact.birthDate = birthDateEdit->date();
    contact.email = emailEdit->text().trimmed();
    contact.phoneNumbers.clear();
    contact.phoneNumbers.append(phoneEdit->text().trimmed());

    if (contact.isValid()) {
        contactList->item(currentRow)->setText(contact.firstName + " " + contact.lastName);
        saveContacts();
    }
    else {
        QMessageBox::warning(this, "Error", "Incorrect data!");
    }
}

void PhonebookApp::deleteContact() {
    int currentRow = contactList->currentRow();
    if (currentRow < 0) return;

    contacts.removeAt(currentRow);
    delete contactList->takeItem(currentRow);
    saveContacts();
}

void PhonebookApp::sortContacts() {
    std::sort(contacts.begin(), contacts.end(), [](const Contact& a, const Contact& b) {
        return a.lastName < b.lastName; // Сортировка по фамилии
        });
    contactList->clear();
    for (const Contact& contact : contacts) {
        contactList->addItem(contact.firstName + " " + contact.lastName);
    }
}

void PhonebookApp::searchContacts() {
    QString searchTerm = QInputDialog::getText(this, "Search", "Type any information about contact:");
    contactList->clear();
    for (const Contact& contact : contacts) 
    {
        if (contact.firstName.contains(searchTerm, Qt::CaseInsensitive) ||
            contact.lastName.contains(searchTerm, Qt::CaseInsensitive) ||
            contact.middleName.contains(searchTerm, Qt::CaseInsensitive) ||
            contact.address.contains(searchTerm, Qt::CaseInsensitive) ||
            contact.email.contains(searchTerm, Qt::CaseInsensitive) ||
            contact.phoneNumbers.contains(searchTerm, Qt::CaseInsensitive))
        { 
            contactList->addItem(contact.firstName + " " + contact.lastName + " " + contact.middleName + "\n" + \
                "address: " + contact.address + "\n" + \
            "Birthdate: " + contact.birthDate.toString("dd.MM.yyyy") + "\n" + \
            "phone numbers: " + contact.phoneNumbers[0]);
            
        }
        
    }
}



void PhonebookApp::loadContacts() {
    QFile file("contacts.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        Contact contact;
        QString line = in.readLine();
        QStringList fields = line.split(" ", QString::SkipEmptyParts);

        if (fields.size() < 6) continue; 

        contact.firstName = fields[0];
        contact.lastName = fields[1];
        contact.middleName = fields[2];
        contact.address = fields[3];
        contact.birthDate = QDate::fromString(fields[4], "yyyy-MM-dd");
        contact.email = fields[5];

        // Считываем оставшиеся телефонные номера
        for (int i = 6; i < fields.size(); ++i) {
            contact.phoneNumbers.append(fields[i]);
        }

        contacts.append(contact);
        dbManager.addContact(contact);
    }
    file.close();
}

void PhonebookApp::saveContacts() {
    QList<Contact> contacts = dbManager.getAllContacts();
    QFile file("contacts.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QTextStream out(&file);
    //QList<Contact> contacts = dbManager.getAllContacts();
    for (const Contact& contact : contacts) {
        out << contact.firstName << " " << contact.lastName << " " << contact.middleName << " "
            << contact.address << " " << contact.birthDate.toString("yyyy-MM-dd") << " "
            << contact.email << " ";
        for (const QString& phone : contact.phoneNumbers) {
            out << phone << " ";
        }
        out << "\n";
    }
    file.close();
}