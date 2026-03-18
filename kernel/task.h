#ifndef TASK_H
#define TASK_H

#include <stdint.h>

typedef struct task {
    uint32_t esp;
    uint32_t ebp;
    uint32_t eip;
    struct task* next;
} task_t;

extern task_t* current_task;

void task_init();
void task_switch(uint32_t esp, uint32_t ebp, uint32_t eip);

#endif

