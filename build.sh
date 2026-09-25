#!/bin/bash
set -e
mkdir -p build
CFLAGS="-m32 -ffreestanding -fno-pie -fno-pic"

# Bootloader
nasm -f bin bootloader/boot.asm -o build/boot.bin

# Kernel
nasm -f elf32 kernel/kernel_entry.asm -o build/kernel_entry.o
gcc $CFLAGS -c kernel/kernel.c -o build/kernel.o
gcc $CFLAGS -c kernel/vga.c -o build/vga.o
nasm -f elf32 kernel/isr.asm -o build/isr.o
gcc $CFLAGS -c kernel/idt.c -o build/idt.o
gcc $CFLAGS -c kernel/pic.c -o build/pic.o

gcc $CFLAGS -c kernel/keyboard.c -o build/keyboard.o
gcc $CFLAGS -c kernel/timer.c -o build/timer.o
nasm -f elf32 kernel/task_switch.asm -o build/task_switch.o
gcc $CFLAGS -c kernel/task.c -o build/task.o
gcc $CFLAGS -c kernel/scheduler.c -o build/scheduler.o
gcc $CFLAGS -c kernel/tasks.c -o build/tasks.o
gcc $CFLAGS -c kernel/paging.c -o build/paging.o
gcc $CFLAGS -c kernel/heap.c -o build/heap.o
gcc $CFLAGS -c kernel/pagefault.c -o build/pagefault.o
nasm -f elf32 kernel/enter_user_mode.asm -o build/enter_user_mode.o
gcc $CFLAGS -c kernel/user.c -o build/user.o
gcc $CFLAGS -c kernel/gpfault.c -o build/gpfault.o
gcc $CFLAGS -c kernel/gdt.c -o build/gdt.o
nasm -f elf32 kernel/gdt_flush.asm -o build/gdt_flush.o
gcc $CFLAGS -c kernel/tss.c -o build/tss.o
gcc $CFLAGS -c kernel/memory.c -o build/memory.o
nasm -f elf32 kernel/isr_syscall.asm -o build/isr_syscall.o
gcc $CFLAGS -c kernel/syscall.c -o build/syscall.o

ld -m elf_i386 -T kernel/linker.ld \
  build/kernel_entry.o build/kernel.o build/vga.o \
  build/idt.o build/keyboard.o build/pic.o \
  build/timer.o build/isr.o \
  build/task.o build/scheduler.o build/task_switch.o \
  build/tasks.o \
  build/paging.o build/heap.o build/pagefault.o \
  build/enter_user_mode.o build/user.o \
  build/gpfault.o \
  build/gdt.o build/gdt_flush.o build/tss.o \
  build/memory.o \
  build/isr_syscall.o build/syscall.o \
  -o build/kernel.bin --oformat binary






# OS Image
cat build/boot.bin build/kernel.bin > os.bin
