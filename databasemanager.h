#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QList>
#include <sqlite3.h>
#include "Contact.h"

class DatabaseManager {
public:
    DatabaseManager(const QString& dbPath);
    ~DatabaseManager();

    bool addContact(const Contact& contact);
    bool editContact(int id, const Contact& contact);
    bool deleteContact(int id);
    QList<Contact> getAllContacts();
    Contact getContact(int id);

private:
    sqlite3* db;
    QString dbPath;

    bool executeSQL(const QString& sql);
    void createTable();
};

#endif // DATABASEMANAGER_H