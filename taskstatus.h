#ifndef TASKSTATUS_H
#define TASKSTATUS_H

enum TaskStatus {
    DONE,
    IN_PROGRESS,
    NOT_DONE
};

TaskStatus getTaskStatus(int status);

int getTaskStatusInt(TaskStatus status);

#endif // TASKSTATUS_H
