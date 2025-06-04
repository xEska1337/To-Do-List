#ifndef TASKMANAGER_H
#define TASKMANAGER_H
#include "task.h"
#include <vector>

namespace TaskManager {

    bool createTask(const Task& task);
    std::vector<Task> getTasksForUser(uint32_t userId);
    Task getTask(uint64_t id);
    bool updateTask(const Task& task);
    bool deleteTask(const Task& task);

}

#endif // TASKMANAGER_H
