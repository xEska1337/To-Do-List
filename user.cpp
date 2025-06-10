#include "user.h"

User::User() {}

User::User(uint32_t id, const std::string& username, const std::string &password){
    this->setId(id);
    this->setUsername(username);
    this->setPassword(password);
    this->setCreationDate(QDate::currentDate());
}

User::User(uint32_t id, const std::string& username, const std::string &password, const QDate &creationDate){
    this->setId(id);
    this->setUsername(username);
    this->setPassword(password);
    this->setCreationDate(creationDate);
}

User::User(uint32_t id, const std::string& username, std::string& password){
    this->setId(id);
    this->setUsername(username);
    this->setPassword(password);
    this->setCreationDate(QDate::currentDate());
}

User::User(uint32_t id, const std::string& username, std::string& password, const QDate &creationDate){
    this->setId(id);
    this->setUsername(username);
    this->setPassword(password);
    this->setCreationDate(creationDate);
}

User::User(const std::string& username, const std::string &password){
    this->setUsername(username);
    this->setPassword(password);
    this->setCreationDate(QDate::currentDate());
}

User::User(const std::string& username, const std::string &password, const QDate &creationDate){
    this->setUsername(username);
    this->setPassword(password);
    this->setCreationDate(creationDate);
}

User::User(const std::string& username, std::string &password){
    this->setUsername(username);
    this->setPassword(password);
    this->setCreationDate(QDate::currentDate());
}

User::User(const std::string& username, std::string &password, const QDate &creationDate){
    this->setUsername(username);
    this->setPassword(password);
    this->setCreationDate(creationDate);
}

User::User(const std::string& username){
    this->setUsername(username);
    this->setCreationDate(QDate::currentDate());
}

void User::setId(uint32_t id){
    this->id = id;
}

void User::setUsername(const std::string &username){
    this->username = username;
}

void User::setPassword(const std::string& password){
    QCryptographicHash hasher(QCryptographicHash::Blake2b_256);
    hasher.addData(QByteArray::fromStdString(password));
    this->password = hasher.result().toHex().toStdString();
}

void User::setPassword(std::string& password){
    this->password = password;
}

void User::setCreationDate(const QDate &creationDate) {
    this->creationDate = creationDate;
}


uint32_t User::getId() const {
    return this->id;
}

std::string User::getUsername() const { // Make const
    return this->username;
}

std::string User::getPassword() const {
    return this->password;
}

QDate User::getCreationDate() const {
    return this->creationDate;
}