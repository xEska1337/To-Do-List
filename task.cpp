#include "task.h"

Task::Task() {}

Task::Task(uint32_t id, const std::string &name, const std::string &description, uint8_t priority, uint32_t teamId, uint32_t userId, TaskStatus status, time_t deadline) {
    this->setId(id);
    this->setName(name);
    this->setDescription(description);
    this->setPriority(priority);
    this->setTeamId(teamId);
    this->setUserId(userId);
    this->setStatus(status);
}

Task::Task(const std::string &name, const std::string &description, uint8_t priority, uint32_t teamId, uint32_t userId, TaskStatus status, time_t deadline) {
    this->setName(name);
    this->setDescription(description);
    this->setPriority(priority);
    this->setTeamId(teamId);
    this->setUserId(userId);
}

void Task::setId(uint64_t id) {
    this->id = id;
}

void Task::setName(const std::string &name) {
    this->name = name;
}

void Task::setDescription(const std::string &description) {
    this->description = description;
}

void Task::setPriority(uint8_t priority) {
    this->priority = priority;
}

void Task::setTeamId(uint32_t teamId) {
    this->teamId = teamId;
}

void Task::setUserId(uint32_t userId) {
    this->userId = userId;
}

void Task::setStatus(int status) {
    this->setStatus(getTaskStatus(status));
}


void Task::setStatus(TaskStatus status) {
    this->status = status;
}

void Task::setDeadline(time_t deadline) {
    this->deadline = deadline;
}

uint64_t Task::getId() const {
    return this->id;
}

std::string Task::getName() const{
    return this->name;
}

std::string Task::getDescription() const {
    return this->description;
}

uint8_t Task::getPriority() const {
    return this->priority;
}

uint32_t Task::getTeamId() const {
    return this->teamId;
}

uint32_t Task::getUserId() const {
    return this->userId;
}

int Task::getStatusAsInt() const {
    return getTaskStatusInt(this->status);
}


TaskStatus Task::getStatus() const {
    return this->status;
}

time_t Task::getDeadline() const {
    return this->deadline;
}