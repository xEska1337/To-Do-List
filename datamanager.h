#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QSqlQuery>
#include <QSqlRecord>

#include "user.h"

class DataManager {
private:
    static QSqlDatabase database;
public:
    static void setDatabase(const QSqlDatabase &database);
    static QSqlDatabase getDatabase();
    static void createUser(User& user);
    static User getUser(uint32_t userid);
    static void deleteUser(const User &user);
    static void updateUser(const User &user);

};

#endif // DATAMANAGER_H
