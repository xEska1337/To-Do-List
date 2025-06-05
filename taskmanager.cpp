#include "taskmanager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDateTime>
#include <vector>

using namespace TaskManager;
bool TaskManager::createTask(const Task& task) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database connection failed!");
        return false;
    }
    QSqlQuery insertQuery(db);
    insertQuery.prepare("INSERT INTO tasks (userAssigned, name, dueDate, description) VALUES (?, ?, ?, ?)");
    insertQuery.addBindValue(task.getUserId());
    insertQuery.addBindValue(QString::fromStdString(task.getName()));
    insertQuery.addBindValue(QDateTime::fromSecsSinceEpoch(task.getDeadline()));
    insertQuery.addBindValue(QString::fromStdString(task.getDescription()));
    if (!insertQuery.exec()) {
        QMessageBox::critical(nullptr, "Database Error", "Failed to create task: " + insertQuery.lastError().text());
        return false;
    }
    return true;
}
bool TaskManager::deleteTask(const Task &task)
{
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