#ifndef TASK_H
#define TASK_H

#include <cstdint>
#include <qtypes.h>
#include <string>

#include "taskstatus.h"

class Task
{
private:
    uint64_t id;
    std::string name;
    std::string description;
    uint8_t priority;
    uint32_t teamId; // If teamId is 0, that means it's Task specified only for one User
    uint32_t userId; // If userId is 0, that means it's Task specified for Team
    TaskStatus status;
    time_t deadline; // If 0 - Task with unlimited time.
public:
    // Constructors
    Task();
    Task(uint32_t id, const std::string &name, const std::string &description, uint8_t priority, uint32_t teamId, uint32_t userId, TaskStatus status, time_t deadline);
    Task(const std::string &name, const std::string &description, uint8_t priority, uint32_t teamId, uint32_t userId, TaskStatus status, time_t deadline);

    // Setters
    void setId(uint64_t id);
    void setName(const std::string &name);
    void setDescription(const std::string &description);
    void setPriority(uint8_t priority);
    void setTeamId(uint32_t teamId);
    void setUserId(uint32_t userId);
    void setStatus(int status);
    void setStatus(TaskStatus status);
    void setDeadline(time_t deadline);

    // Getters
    uint64_t getId() const;
    std::string getName();
    std::string getDescription();
    uint8_t getPriority() const;
    uint32_t getTeamId() const;
    uint32_t getUserId() const;
    int getStatusAsInt() const;
    TaskStatus getStatus() const;
    time_t getDeadline() const;

};

#endif // TASK_H
