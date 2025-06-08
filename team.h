#ifndef TEAM_H
#define TEAM_H

#include <cstdint>
#include <string>
#include <vector>

#include "user.h"
#include "usermanager.h"

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
    std::string getName() const;
    uint64_t getPassword() const;
    std::vector<uint32_t> getMembers() const;
    std::vector<User> getMembersAsUsers() const;
    bool containsUser(uint32_t userid) const;
    bool containsUser(const User &user) const;

    // Adding members
    void addMember(const User &user);
    void addMember(uint32_t userid);
    void removeMember(const User &user);
    void removeMember(uint32_t userid);

};

#endif // TEAM_H
