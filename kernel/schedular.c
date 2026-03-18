#include "schedular.h"
#include "task.h"

void schedular_tick() {
    if (!current_task || !current_task->next)
        return;

    current_task = current_task->next;   // 🔥 MOVE TO NEXT TASK

    task_switch(
        current_task->esp,
        current_task->ebp,
        current_task->eip
    );
}

