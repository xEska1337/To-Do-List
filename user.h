#ifndef USER_H
#define USER_H

#include <cstdint>
#include <string>
#include <QDate>

class User
{
private:
    uint32_t id;
    std::string username;
    uint64_t password;
    QDate creationDate;
public:
    // Constructors
    User();
    User(uint32_t id, const std::string& username, const std::string &password);
    User(uint32_t id, const std::string& username, const std::string &password, const QDate &creationDate);
    User(uint32_t id, const std::string& username, uint64_t password);
    User(uint32_t id, const std::string& username, uint64_t password, const QDate &creationDate);
    User(const std::string& username, const std::string &password);
    User(const std::string& username, const std::string &password, const QDate &creationDate);
    User(const std::string& username, uint64_t password);
    User(const std::string& username, uint64_t password, const QDate &creationDate);
    explicit User(const std::string& username);

    // Setters
    void setId(uint32_t id);
    void setUsername(const std::string &username);
    void setPassword(const std::string &password);
    void setPassword(uint64_t password);
    void setCreationDate(const QDate &creationDate);

    // Getters
    uint32_t getId() const;
    std::string getUsername() const; // Make const
    uint64_t getPassword() const;
    QDate getCreationDate() const;
};

#endif // USER_H