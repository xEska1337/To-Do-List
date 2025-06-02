#include "user.h"

User::User() {}

User::User(uint32_t id, const std::string& username, const std::string &password, const QDate &creationDate){
    this->setId(id);
    this->setUsername(username);
    this->setPassword(password);
    this->setCreationDate(creationDate);
}

User::User(uint32_t id, const std::string& username, const std::string &password){
    this->setId(id);
    this->setUsername(username);
    this->setPassword(password);
    this->setCreationDate(QDate::currentDate());
}

User::User(uint32_t id, const std::string& username, uint64_t password, const QDate &creationDate){
    this->setId(id);
    this->setUsername(username);
    this->setPassword(password);
    this->setCreationDate(creationDate);
}

User::User(uint32_t id, const std::string& username, uint64_t password){
    this->setId(id);
    this->setUsername(username);
    this->setPassword(password);
    this->setCreationDate(QDate::currentDate());
}

User::User(const std::string& username, const std::string &password, const QDate &creationDate) : id(0){
    this->setUsername(username);
    this->setPassword(password);
    this->setCreationDate(creationDate);
}

User::User(const std::string& username, const std::string &password) : id(0){
    this->setUsername(username);
    this->setPassword(password);
    this->setCreationDate(QDate::currentDate());
}

User::User(const std::string& username, uint64_t password, const QDate &creationDate) : id(0){
    this->setUsername(username);
    this->setPassword(password);
    this->setCreationDate(creationDate);
}

User::User(const std::string& username, uint64_t password) : id(0){
    this->setUsername(username);
    this->setPassword(password);
    this->setCreationDate(QDate::currentDate());
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

void User::setCreationDate(const QDate &creationDate) {
    this->creationDate = creationDate;
}

uint32_t User::getId() const {
    return this->id;
}

std::string User::getUsername() const{
    return this->username;
}

QDate User::getCreationDate() const {
    return this->creationDate;
}


uint64_t User::getPassword() const {
    return this->password;
}

