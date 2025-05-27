#include "taskstatus.h"

TaskStatus getTaskStatus(int status) {
    switch (status) {
        case 1:
            return IN_PROGRESS;
        case 2:
            return DONE;
        default:
            return NOT_DONE;
    }
}

int getTaskStatusInt(TaskStatus status) {
    switch (status) {
        case DONE:
            return 2;
        case IN_PROGRESS:
            return 1;
        default:
            return 0;
    }
}