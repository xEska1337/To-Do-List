#ifndef TASKMANAGER_H
#define TASKMANAGER_H
#include "task.h"

namespace TaskManager {

    bool createTask(const Task& task);
    Task getTask(uint64_t id);
    bool updateTask(const Task& task);
    bool deleteTask(const Task& task);

}

#endif // TASKMANAGER_H
