#include "databasemanager.h"
#include <QDebug>

DatabaseManager::DatabaseManager(const QString& dbPath) : dbPath(dbPath), db(nullptr) {
    if (sqlite3_open(dbPath.toUtf8().constData(), &db) != SQLITE_OK) {
        qDebug() << "Error opening database:" << sqlite3_errmsg(db);
    }
    else {
        createTable();
    }
}

DatabaseManager::~DatabaseManager() {
    if (db) sqlite3_close(db);
}

void DatabaseManager::createTable() {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS contacts (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            first_name TEXT NOT NULL,
            last_name TEXT NOT NULL,
            middle_name TEXT,
            address TEXT,
            birth_date TEXT,
            email TEXT,
            phone_numbers TEXT
        )
    )";
    executeSQL(sql);
}


QList<Contact> DatabaseManager::getAllContacts() {
    QList<Contact> contacts;
    const char* sql = "SELECT * FROM contacts";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Contact contact;
            contact.firstName = QString(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            contact.lastName = QString(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            contact.middleName = QString(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
            contact.address = QString(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
            contact.birthDate = QDate::fromString(QString(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5))), "yyyy-MM-dd");
            contact.email = QString(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)));

            // Разделяем номера телефонов (если они хранятся через ";")
            QString phonesStr = QString(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7)));
            contact.phoneNumbers = phonesStr.split(";", QString::SkipEmptyParts);

            contacts.append(contact);
        }
        sqlite3_finalize(stmt);
    }
    else {
        qDebug() << "Ошибка при выполнении SQL-запроса:" << sqlite3_errmsg(db);
    }

    return contacts;
}
bool DatabaseManager::addContact(const Contact& contact) {
    QString phones = contact.phoneNumbers.join(";");
    QString sql = QString("INSERT INTO contacts (first_name, last_name, middle_name, address, birth_date, email, phone_numbers) "
        "VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7')")
        .arg(contact.firstName).arg(contact.lastName).arg(contact.middleName)
        .arg(contact.address).arg(contact.birthDate.toString("yyyy-MM-dd"))
        .arg(contact.email).arg(phones);
    return executeSQL(sql);
}

// 

bool DatabaseManager::executeSQL(const QString& sql) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.toUtf8().constData(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        qDebug() << "SQL error:" << errMsg;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}