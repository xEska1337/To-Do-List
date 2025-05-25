#ifndef TASK_H
#define TASK_H

#include <cstdint>
#include <string>

class Task
{
private:
    uint32_t id;
    std::string name;
    std::string description;
    uint32_t priority;
    uint32_t teamid;
    uint32_t userid;
    uint32_t status;
    uint32_t deadline;
public:
    // Constructors
    Task();

    // TODO: Add constructors, getters, setters etc.
};

#endif // TASK_H
