#ifndef PAGEFAULT_H
#define PAGEFAULT_H

#include <stdint.h>

void page_fault_handler(uint32_t fault_addr);

#endif

