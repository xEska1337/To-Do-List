#include "taskmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDateTime>
#include <vector>

using namespace TaskManager;

/**
 * Creates Task in a database
 * @param task Task to create in a database
 * @return True if success, false if there was any error
 */
bool TaskManager::createTask(const Task& task) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database connection failed!");
        return false;
    }
    QSqlQuery insertQuery(db);
    insertQuery.prepare("INSERT INTO tasks (userAssigned, teamId, name, dueDate, description) VALUES (?, ?, ?, ?, ?)");
    insertQuery.addBindValue(task.getUserId());
    insertQuery.addBindValue(task.getTeamId());
    insertQuery.addBindValue(QString::fromStdString(task.getName()));
    insertQuery.addBindValue(QDateTime::fromSecsSinceEpoch(task.getDeadline()));
    insertQuery.addBindValue(QString::fromStdString(task.getDescription()));
    if (!insertQuery.exec()) {
        QMessageBox::critical(nullptr, "Database Error", "Failed to create task: " + insertQuery.lastError().text());
        return false;
    }
    return true;
}

/**
 * Deletes Task from a database
 * @param task Task to delete
 * @return True if success, false if Task doesn't exist or there was any error
 */
bool TaskManager::deleteTask(const Task &task) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database connection failed!");
        return false;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM tasks WHERE id = ?");
    query.addBindValue(task.getId());

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Database Error",
                              "Failed to delete task: " + query.lastError().text());
        return false;
    }
    return true;
}

/**
 * Returns all User's Task
 * @param userId User's ID
 * @return Vector of User's Tasks
 */
std::vector<Task> TaskManager::getTasksForUser(uint32_t userId) {
    std::vector<Task> tasks;
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) return tasks;
    QSqlQuery query(db);
    query.prepare("SELECT * FROM tasks WHERE userAssigned = ?");
    query.addBindValue(userId);
    if (!query.exec()) return tasks;
    while (query.next()) {
        Task task;
        task.setId(query.value("id").toULongLong());
        task.setUserId(query.value("userAssigned").toUInt());
        task.setName(query.value("name").toString().toStdString());
        task.setDescription(query.value("description").toString().toStdString());
        QDateTime dueDateTime = query.value("dueDate").toDateTime();
        task.setDeadline(dueDateTime.toSecsSinceEpoch());
        tasks.push_back(task);
    }
    return tasks;
}

/**
 * Returns all Team's Task
 * @param teamId Team's ID
 * @return Vector of Team's Tasks
 */
std::vector<Task> TaskManager::getTasksForTeam(uint32_t teamId) {
    std::vector<Task> tasks;
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) return tasks;

    QSqlQuery query(db);
    query.prepare("SELECT * FROM tasks WHERE teamId = ? AND userAssigned = 0");
    query.addBindValue(teamId);

    if (!query.exec()) return tasks;

    while (query.next()) {
        Task task;
        task.setId(query.value("id").toULongLong());
        task.setUserId(query.value("userAssigned").toUInt());
        task.setTeamId(query.value("teamId").toUInt());
        task.setName(query.value("name").toString().toStdString());
        task.setDescription(query.value("description").toString().toStdString());
        QDateTime dueDateTime = query.value("dueDate").toDateTime();
        task.setDeadline(dueDateTime.toSecsSinceEpoch());
        tasks.push_back(task);
    }
    return tasks;
}

/**
 * Gets Task from a database identifying it by ID and returns as Task's object
 * @param id Task's ID
 * @return Task's object, if Task->id is 0, then Task doesn't exist, or there was an error
 */
Task TaskManager::getTask(uint64_t id) {

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database connection failed!");
        return Task(0);
    }

    QSqlQuery query(db);
    query.prepare("SELECT * FROM tasks WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Database Error",
                              "Failed to get task: " + query.lastError().text());
        return Task(0);
    }

    while (query.next()) {
        Task task;
        task.setId(query.value("id").toULongLong());
        task.setUserId(query.value("userAssigned").toUInt());
        task.setTeamId(query.value("teamId").toUInt());
        task.setName(query.value("name").toString().toStdString());
        task.setDescription(query.value("description").toString().toStdString());
        QDateTime dueDateTime = query.value("dueDate").toDateTime();
        task.setDeadline(dueDateTime.toSecsSinceEpoch());
        return task;
    }

    return Task(0);
}

/**
 * Updates Task in a database
 * @param task Task to update
 * @return True if success, false if there was any error
 */
bool TaskManager::updateTask(const Task& task) {

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database connection failed!");
        return false;
    }

    // Check if task exists
    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT COUNT(*) FROM tasks WHERE id = ?");
    checkQuery.addBindValue(task.getId());

    if (!checkQuery.exec()) {
        QMessageBox::critical(nullptr, "Database Error",
                             "Failed to check id: " + checkQuery.lastError().text());
        return false;
    }

    checkQuery.next();
    if (checkQuery.value(0).toInt() <= 0) {
        QMessageBox::warning(nullptr, "Error", "Task doesn't exist!");
        return false;
    }

    QSqlQuery updateQuery(db);
    updateQuery.prepare("UPDATE tasks SET userAssigned = ? teamId = ? name = ? dueDate = ? description = ? WHERE id = ?");
    updateQuery.addBindValue(task.getUserId());
    updateQuery.addBindValue(task.getTeamId());
    updateQuery.addBindValue(QString::fromStdString(task.getName()));
    updateQuery.addBindValue(QDateTime::fromSecsSinceEpoch(task.getDeadline()));
    updateQuery.addBindValue(QString::fromStdString(task.getDescription()));
    updateQuery.addBindValue(task.getId());
    if (!updateQuery.exec()) {
        QMessageBox::critical(nullptr, "Database Error", "Failed to update task: " + updateQuery.lastError().text());
        return false;
    }
    return true;
}