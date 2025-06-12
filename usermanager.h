#ifndef USERMANAGER_H
#define USERMANAGER_H
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "user.h"

namespace UserManager {
    /**
     * @brief Creates User in a database
     *
     * @param user User to create in a database
     * @return True if success, false if there was any error
     */
    bool createUser(const User& user);

    /**
     * @brief Gets User from a database identifying him by username and returns as User's object
     *
     * @param username User's name
     * @return User's object, if User->id equals 0, then User doesn't exist, or there was an error
     */
    User getUser(const std::string& username);

    /**
     * @brief Gets User from a database identifying him by username and returns as User's object
     *
     * @param username User's name
     * @return User's object, if User->id is 0, then User doesn't exist, or there was an error
     */
    User getUser(uint32_t id);

    /**
     * @brief Updates User in a database
     *
     * @param user User to update
     * @return True if success, false if there was any error
     */
    bool updateUser(const User& user);

    /**
     * @brief Deletes User from a database
     *
     * @param user User to delete
     * @return True if success, false if User doesn't exist or there was any error
     */
    bool deleteUser(const User& user);

    /**
     * @brief Deletes User using his ID from a database
     *
     * @param id ID of a User to delete
     * @return True if success, false if User doesn't exist or there was any error
     */
    bool deleteUser(uint32_t id);

    /**
     * @brief Returns all Users from a database
     *
     * @return Vector of all Users from a database
     */
    std::vector<User> getAllUsers();


}

#endif // USERMANAGER_H
