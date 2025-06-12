#ifndef TASKMANAGER_H
#define TASKMANAGER_H
#include "task.h"
#include <vector>

namespace TaskManager {

    /**
     * @brief Creates Task in a database
     *
     * @param task Task to create in a database
     * @return True if success, false if there was any error
     */
    bool createTask(const Task& task);

    /**
     * @brief Returns all Team's Task
     *
     * @param teamId Team's ID
     * @return Vector of Team's Tasks
     */
    std::vector<Task> getTasksForTeam(uint32_t teamId);

    /**
     * @brief Returns all User's Task
     *
     * @param userId User's ID
     * @return Vector of User's Tasks
     */
    std::vector<Task> getTasksForUser(uint32_t userId);

    /**
     * @brief Gets Task from a database identifying it by ID and returns as Task's object
     *
     * @param id Task's ID
     * @return Task's object, if Task->id is 0, then Task doesn't exist, or there was an error
     */
    Task getTask(uint64_t id);

    /**
     * @brief Updates Task in a database
     *
     * @param task Task to update
     * @return True if success, false if there was any error
     */
    bool updateTask(const Task& task);

    /**
     * @brief Deletes Task from a database
     *
     * @param task Task to delete
     * @return True if success, false if Task doesn't exist or there was any error
     */
    bool deleteTask(const Task& task);

}

#endif // TASKMANAGER_H
