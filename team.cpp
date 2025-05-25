#include "team.h"

#include <algorithm>

Team::Team() {}

Team::Team(uint32_t id, const std::string &name, uint64_t password, const std::vector<uint32_t> &members) {
    this->setId(id);
    this->setName(name);
    this->setPassword(password);
    this->setMembers(members);
}

Team::Team(uint32_t id, const std::string &name, const std::string &password, const std::vector<uint32_t> &members) {
    this->setId(id);
    this->setName(name);
    this->setPassword(password);
    this->setMembers(members);
}

Team::Team(uint32_t id, const std::string &name, uint64_t password) {
    this->setId(id);
    this->setName(name);
    this->setPassword(password);
}

Team::Team(uint32_t id, const std::string &name, const std::string &password) {
    this->setId(id);
    this->setName(name);
    this->setPassword(password);
}

Team::Team(const std::string &name, uint64_t password, const std::vector<uint32_t> &members) {
    this->setName(name);
    this->setPassword(password);
    this->setMembers(members);
}

Team::Team(const std::string &name, const std::string &password, const std::vector<uint32_t> &members) {
    this->setName(name);
    this->setPassword(password);
    this->setMembers(members);
}

void Team::setId(uint32_t id) {
    this->id = id;
}

void Team::setName(const std::string &name) {
    this->name = name;
}

void Team::setPassword(uint64_t password) {
    this->password = password;
}

void Team::setPassword(const std::string &password) {
    std::hash<std::string> hasher;
    this->password = hasher(password);
}


void Team::setMembers(const std::vector<uint32_t> &members) {
    this->members = members;
}

uint32_t Team::getId() const {
    return this->id;
}

std::string Team::getName() {
    return this->name;
}

uint64_t Team::getPassword() const {
    return this->password;
}

std::vector<uint32_t> Team::getMembers() {
    return this->members;
}

std::vector<User> Team::getMembersAsUsers() {

    // TODO: Add a manager that will create a new vector of users using vector of their id

    return {};
}

bool Team::containsUser(uint32_t userid) {
    return std::find(this->members.begin(), this->members.end(), userid) != this->members.end();
}

bool Team::containsUser(const User &user) {
    return std::find(this->members.begin(), this->members.end(), user.getId()) != this->members.end();
}

