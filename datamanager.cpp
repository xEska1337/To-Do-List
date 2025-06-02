#include "datamanager.h"

QSqlDatabase DataManager::database;

void DataManager::setDatabase(const QSqlDatabase &database) {
    DataManager::database = database;
}

QSqlDatabase DataManager::getDatabase() {
    return database;
}

void DataManager::createUser(User& user) {
    QSqlQuery query(DataManager::database);
    if (getUser(user.getId()).getPassword() != 0) {
        updateUser(user);
        return;
    }
    query.prepare("INSERT INTO `users` (`username`, `password`, `creationDate`) VALUES (:username, :password, :creationDate)");
    query.bindValue(":username", QString::fromStdString(user.getUsername()));
    query.bindValue(":password", user.getPassword());
    query.bindValue(":creationDate", user.getCreationDate());
    query.exec();
    user.setId(query.lastInsertId().toUInt());
}

User DataManager::getUser(uint32_t userid) {
    QSqlQuery query(DataManager::database);
    query.prepare("SELECT * FROM `users` WHERE `id`=:id");
    query.bindValue(":id", userid);
    query.exec();
    if (!query.next()) {
        return User{0, "", 0};
    }
    User user = User(query.value("id").toUInt(), query.value("username").toString().toStdString(), query.value("password").toULongLong(), query.value("creationDate").toDate());
    return user;
}

void DataManager::deleteUser(const User &user) {
    QSqlQuery query(DataManager::database);
    query.prepare("DELETE FROM `users` WHERE `id`=:id");
    query.bindValue(":id", user.getId());
    query.exec();
}

void DataManager::updateUser(const User &user) {
    QSqlQuery query(DataManager::database);
    query.prepare("UPDATE `users` SET `username`=:username, `password`=:password, `creationDate`=:creationDate WHERE `id`=:id");
    query.bindValue(":username", QString::fromStdString(user.getUsername()));
    query.bindValue(":password", user.getPassword());
    query.bindValue(":creationDate", user.getCreationDate());
    query.bindValue(":id", user.getId());
    query.exec();
}
