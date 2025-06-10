#include "team.h"

Team::Team() {}

Team::Team(uint32_t id, const std::string &name, std::string &password, const std::vector<uint32_t> &members) {
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

Team::Team(uint32_t id, const std::string &name, std::string &password) {
    this->setId(id);
    this->setName(name);
    this->setPassword(password);
}

Team::Team(uint32_t id, const std::string &name, const std::string &password) {
    this->setId(id);
    this->setName(name);
    this->setPassword(password);
}

Team::Team(const std::string &name, std::string &password, const std::vector<uint32_t> &members) {
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

void Team::setPassword(std::string &password) {
    this->password = password;
}

void Team::setPassword(const std::string &password) {
    QCryptographicHash hasher(QCryptographicHash::Blake2b_256);
    hasher.addData(QByteArray::fromStdString(password));
    this->password = hasher.result().toHex().toStdString();
}


void Team::setMembers(const std::vector<uint32_t> &members) {
    this->members = members;
}

uint32_t Team::getId() const {
    return this->id;
}

std::string Team::getName() const {
    return this->name;
}

std::string Team::getPassword() const {
    return this->password;
}

std::vector<uint32_t> Team::getMembers() const {
    return this->members;
}

std::vector<User> Team::getMembersAsUsers() const {

    std::vector<User> users;

    for (uint32_t userid : this->members) {
        users.push_back(UserManager::getUser(userid));
    }

    return users;
}

bool Team::containsUser(uint32_t userid) const {
    return std::find(this->members.begin(), this->members.end(), userid) != this->members.end();
}

bool Team::containsUser(const User &user) const {
    return std::find(this->members.begin(), this->members.end(), user.getId()) != this->members.end();
}

void Team::addMember(const User &user) {
    this->addMember(user.getId());
}

void Team::addMember(uint32_t userid) {
    if (this->containsUser(userid)) return;
    this->members.push_back(userid);
}

void Team::removeMember(const User &user) {
    this->removeMember(user.getId());
}


void Team::removeMember(uint32_t userid) {
    for (size_t i = 0; i < this->members.size(); i++) {
        if (this->members[i] == userid) {
            this->members.erase(this->members.begin() + i);
        }
    }
}
