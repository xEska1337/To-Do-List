#include "usermanager.h"
#include <vector>

using namespace UserManager;

bool UserManager::createUser(const User& user) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database connection failed!");
        return false;
    }
    // Check if username already exists
    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT COUNT(*) FROM users WHERE username = ?");
    checkQuery.addBindValue(QString::fromStdString(user.getUsername()));

    if (!checkQuery.exec()) {
        QMessageBox::critical(nullptr, "Database Error",
                             "Failed to check username: " + checkQuery.lastError().text());
        return false;
    }

    checkQuery.next();
    if (checkQuery.value(0).toInt() > 0) {
        QMessageBox::warning(nullptr, "Error", "Username already exists!");
        return false;
    }

    // Insert new user
    QSqlQuery insertQuery(db);
    insertQuery.prepare("INSERT INTO users (username, password, creationDate) VALUES (?, ?, ?)");
    insertQuery.addBindValue(QString::fromStdString(user.getUsername()));
    insertQuery.addBindValue(QString::number(user.getPassword()));
    insertQuery.addBindValue(user.getCreationDate());

    if (!insertQuery.exec()) {
        QMessageBox::critical(nullptr, "Database Error",
                             "Failed to create user: " + insertQuery.lastError().text());
        return false;
    }
    return true;
}

User UserManager::getUser(const std::string &username) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database connection failed!");
        return User(0, "", 0);
    }

    QSqlQuery getQuery(db);
    getQuery.prepare("SELECT * FROM users WHERE username = ?");
    getQuery.addBindValue(QString::fromStdString(username));

    if (!getQuery.exec()) {
        QMessageBox::critical(nullptr, "Database Error",
                             "Failed to check username: " + getQuery.lastError().text());
        return User(0, "", 0);
    }

    getQuery.next();
    if (getQuery.value(0).toInt() <= 0) {
        QMessageBox::warning(nullptr, "Error", "User with that username doesn't exist!");
        return User(0, "", 0);
    }

    User user{
        getQuery.value("id").toUInt(),
        getQuery.value("username").toString().toStdString(),
        getQuery.value("password").toULongLong(),
        getQuery.value("creationDate").toDate()
    };

    return user;

}

User UserManager::getUser(uint32_t id) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database connection failed!");
        return User(0, "", 0);
    }

    QSqlQuery getQuery(db);
    getQuery.prepare("SELECT * FROM users WHERE id = ?");
    getQuery.addBindValue(id);

    if (!getQuery.exec()) {
      QMessageBox::critical(nullptr, "Database Error", "Failed to check username: " + getQuery.lastError().text());
        return User(0, "", 0);
    }


    getQuery.next();
    if (getQuery.value(0).toInt() <= 0) {
        QMessageBox::warning(nullptr, "Error", "User with that username doesn't exist!");
        return User(0, "", 0);
    }

    User user{
        getQuery.value("id").toUInt(),
        getQuery.value("username").toString().toStdString(),
        getQuery.value("password").toULongLong(),
        getQuery.value("creationDate").toDate()
    };

    return user;

}

bool UserManager::updateUser(const User &user) {

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database connection failed!");
        return false;
    }

    // Check if user exists
    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT COUNT(*) FROM users WHERE id = ?");
    checkQuery.addBindValue(user.getId());

    if (!checkQuery.exec()) {
        QMessageBox::critical(nullptr, "Database Error",
                             "Failed to check id: " + checkQuery.lastError().text());
        return false;
    }

    checkQuery.next();
    if (checkQuery.value(0).toInt() <= 0) {
        QMessageBox::warning(nullptr, "Error", "User doesn't exist!");
        return false;
    }

    QSqlQuery updateQuery(db);
    updateQuery.prepare("UPDATE users SET username=?, password=?, creationDate=? WHERE id=?");
    updateQuery.addBindValue(QString::fromStdString(user.getUsername()));
    updateQuery.addBindValue(user.getPassword());
    updateQuery.addBindValue(user.getCreationDate());
    updateQuery.addBindValue(user.getId());
    if (!updateQuery.exec()) {
        QMessageBox::critical(nullptr, "Database Error",
                             "Failed to update user: " + updateQuery.lastError().text());
        return false;
    }
    return true;
}

bool UserManager::deleteUser(const User &user) {
    return UserManager::deleteUser(user.getId());
}


bool UserManager::deleteUser(uint32_t id) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database connection failed!");
        return false;
    }

    // Check if user exists
    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT COUNT(*) FROM users WHERE id = ?");
    checkQuery.addBindValue(id);

    if (!checkQuery.exec()) {
        QMessageBox::critical(nullptr, "Database Error",
                             "Failed to check id: " + checkQuery.lastError().text());
        return false;
    }

    if (!checkQuery.next()) {
        QMessageBox::warning(nullptr, "Error", "User doesn't exist!");
        return false;
    }

    QSqlQuery deleteQuery(db);
    deleteQuery.prepare("DELETE FROM users WHERE id = ?");
    deleteQuery.addBindValue(id);
    if (!deleteQuery.exec()) {
        QMessageBox::critical(nullptr, "Database Error",
                             "Failed to delete user: " + deleteQuery.lastError().text());
        return false;
    }

    QSqlQuery deleteTasksQuery(db);
    deleteTasksQuery.prepare("DELETE FROM tasks WHERE userAssigned = ?");
    deleteTasksQuery.addBindValue(id);
    if (!deleteTasksQuery.exec()) {
        QMessageBox::critical(nullptr, "Database Error",
                             "Failed to delete user's tasks: " + deleteQuery.lastError().text());
        return false;
    }

    return true;
}

std::vector<User> UserManager::getAllUsers() {
    std::vector<User> users;
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database connection failed!");
        return users;
    }

    QSqlQuery query(db);
    query.prepare("SELECT id, username, password, creationDate FROM users");

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Database Error",
                             "Failed to retrieve users: " + query.lastError().text());
        return users;
    }

    while (query.next()) {
        User user(
            query.value("id").toUInt(),
            query.value("username").toString().toStdString(),
            query.value("password").toULongLong(), // Matches your User constructor
            query.value("creationDate").toDate()
        );
        users.push_back(user);
    }
    return users;
}