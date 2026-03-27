#include "scheduler.h"
#include "task.h"

void scheduler_tick() {
    if (!current_task || !current_task->next)
        return;

    current_task = current_task->next;

    task_switch(
        current_task->esp,
        current_task->ebp,
        current_task->eip
    );
}

