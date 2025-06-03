#ifndef USERMANAGER_H
#define USERMANAGER_H
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "user.h"

namespace UserManager {

     bool createUser(const User& user);
     User getUser(const std::string& username);
     User getUser(uint32_t id);
     bool updateUser(const User& user);
     bool deleteUser(uint32_t id);

}

#endif // USERMANAGER_H
