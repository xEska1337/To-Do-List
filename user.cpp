#include "user.h"

User::User() {}

User::User(uint32_t id, const std::string& username, const std::string &password){
    this->setId(id);
    this->setUsername(username);
    this->setPassword(password);
}

User::User(uint32_t id, const std::string& username, uint64_t password){
    this->setId(id);
    this->setUsername(username);
    this->setPassword(password);
}

User::User(const std::string& username, const std::string &password){
    this->setUsername(username);
    this->setPassword(password);
}

User::User(const std::string& username, uint64_t password){
    this->setUsername(username);
    this->setPassword(password);
}

User::User(const std::string& username){
    this->setUsername(username);
}

void User::setId(uint32_t id){
    this->id = id;
}

void User::setUsername(const std::string &username){
    this->username = username;
}

void User::setPassword(const std::string& password){
    std::hash<std::string> hasher;
    this->password = hasher(password);
}

void User::setPassword(uint64_t password){
    this->password = password;
}

uint32_t User::getId() const {
    return this->id;
}

std::string User::getUsername() const { // Make const
    return this->username;
}

uint64_t User::getPassword() const {
    return this->password;
}