#ifndef TEAM_H
#define TEAM_H

#include <cstdint>
#include <string>
#include <vector>

#include "user.h"

class Team
{
private:
    uint32_t id;
    std::string name;
    uint64_t password;
    std::vector<uint32_t> members;
public:
    // Constructors
    Team();
    Team(uint32_t id, const std::string &name, uint64_t password, const std::vector<uint32_t> &members);
    Team(uint32_t id, const std::string &name, const std::string &password, const std::vector<uint32_t> &members);
    Team(uint32_t id, const std::string &name, uint64_t password);
    Team(uint32_t id, const std::string &name, const std::string &password);
    Team(const std::string &name, uint64_t password, const std::vector<uint32_t> &members);
    Team(const std::string &name, const std::string &password, const std::vector<uint32_t> &members);

    // Setters
    void setId(uint32_t id);
    void setName(const std::string &name);
    void setPassword(const std::string &password);
    void setPassword(uint64_t password);
    void setMembers(const std::vector<uint32_t> &members);

    // Getters
    uint32_t getId() const;
    std::string getName();
    uint64_t getPassword() const;
    std::vector<uint32_t> getMembers();
    std::vector<User> getMembersAsUsers();
    bool containsUser(uint32_t userid);
    bool containsUser(const User &user);

};

#endif // TEAM_H
