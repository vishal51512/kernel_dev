#include "task.h"

task_t task1, task2;
task_t* current_task;

extern void task1_entry();
extern void task2_entry();

void task_init() {
    task1.esp = 0x80000;
    task1.ebp = 0x80000;
    task1.eip = (uint32_t)task1_entry;
    task1.next = &task2;

    task2.esp = 0x70000;
    task2.ebp = 0x70000;
    task2.eip = (uint32_t)task2_entry;
    task2.next = &task1;

    current_task = &task1;
}

