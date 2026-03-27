#!/bin/bash
set -e
mkdir -p build

# Bootloader
nasm -f bin bootloader/boot.asm -o build/boot.bin

# Kernel
nasm -f elf32 kernel/kernel_entry.asm -o build/kernel_entry.o
gcc -m32 -ffreestanding -c kernel/kernel.c -o build/kernel.o
gcc -m32 -ffreestanding -c kernel/vga.c -o build/vga.o
nasm -f elf32 kernel/isr.asm -o build/isr.o
gcc -m32 -ffreestanding -c kernel/idt.c -o build/idt.o
gcc -m32 -ffreestanding -c kernel/pic.c -o build/pic.o

gcc -m32 -ffreestanding -c kernel/keyboard.c -o build/keyboard.o
gcc -m32 -ffreestanding -c kernel/timer.c -o build/timer.o
nasm -f elf32 kernel/task_switch.asm -o build/task_switch.o
gcc -m32 -ffreestanding -c kernel/task.c -o build/task.o
gcc -m32 -ffreestanding -c kernel/scheduler.c -o build/scheduler.o
gcc -m32 -ffreestanding -c kernel/tasks.c -o build/tasks.o
gcc -m32 -ffreestanding -c kernel/paging.c -o build/paging.o
gcc -m32 -ffreestanding -c kernel/heap.c -o build/heap.o
gcc -m32 -ffreestanding -c kernel/pagefault.c -o build/pagefault.o
nasm -f elf32 kernel/enter_user_mode.asm -o build/enter_user_mode.o
gcc  -m32 -ffreestanding -c kernel/user.c -o build/user.o
gcc -m32 -ffreestanding -c kernel/gpfault.c -o build/gpfault.o
gcc -m32 -ffreestanding -c kernel/gdt.c -o build/gdt.o
nasm -f elf32 kernel/gdt_flush.asm -o build/gdt_flush.o
gcc -m32 -ffreestanding -c kernel/tss.c -o build/tss.o
gcc -m32 -ffreestanding -c kernel/memory.c -o build/memory.o
nasm -f elf32 kernel/isr_syscall.asm -o build/isr_syscall.o
gcc -m32 -ffreestanding -c kernel/syscall.c -o build/syscall.o

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

