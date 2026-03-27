# NovaAI OS — Bare-Metal x86 Kernel

An educational 32-bit x86 operating system kernel written in C and x86 Assembly.

---

## Project Structure

```
kernel_dev/
├── bootloader/
│   ├── boot.asm        — Primary bootloader (real mode → protected mode, VBE)
│   └── boot1.asm       — Minimal debug bootloader (does not load kernel)
├── kernel/
│   ├── kernel.c        — Kernel entry point (kernel_main)
│   ├── kernel_entry.asm— ASM stub that calls kernel_main
│   ├── vga.c / vga.h   — VGA text mode output (with scrolling)
│   ├── gdt.c / gdt.h   — Global Descriptor Table
│   ├── gdt_flush.asm   — Loads GDT and reloads segment registers
│   ├── idt.c / idt.h   — Interrupt Descriptor Table
│   ├── isr.asm         — Low-level interrupt service routines
│   ├── isr_syscall.asm — Syscall (int 0x80) stub
│   ├── pic.c / pic.h   — PIC remapping (IRQ 0–15 → gates 32–47)
│   ├── timer.c / timer.h — PIT timer (100 Hz), drives task scheduler
│   ├── keyboard.c / keyboard.h — PS/2 keyboard handler
│   ├── task.c / task.h — Task control block, task_init()
│   ├── tasks.c         — task1_entry / task2_entry (demo tasks)
│   ├── scheduler.c / scheduler.h — Round-robin scheduler tick
│   ├── task_switch.asm — Low-level ESP/EBP/EIP swap
│   ├── paging.c / paging.h — Identity-mapped paging (first 16 MB)
│   ├── heap.c / heap.h — Bump allocator (kmalloc / kfree)
│   ├── tss.c / tss.h   — Task State Segment (ring 0 stack for ISRs)
│   ├── pagefault.c / pagefault.h — #PF handler (prints fault address)
│   ├── gpfault.c       — #GP handler
│   ├── enter_user_mode.asm — IRETD into Ring 3
│   ├── user.c          — User-mode demo (syscall_write loop)
│   ├── syscall.c       — Syscall dispatcher (int 0x80)
│   ├── memory.c / memory.h — memset helper
│   ├── port.h          — inb / outb helpers
│   └── linker.ld       — Kernel linked at 0x1000
├── build/              — Compiled object files and binaries
├── build.sh            — Full build script
├── build1.sh           — Minimal bootloader-only build
├── kernel.asm          — Legacy 16-bit stub (unused)
└── os.bin              — Final bootable image (boot.bin + kernel.bin)
```

---

## Build Instructions

### Prerequisites

```bash
sudo apt-get install nasm gcc-multilib binutils
```

### Build

```bash
chmod +x build.sh
./build.sh
```

Output: `os.bin` — a raw bootable disk image.

### Run in QEMU

```bash
qemu-system-i386 -drive format=raw,file=os.bin
```

### Debug in QEMU

```bash
qemu-system-i386 -drive format=raw,file=os.bin -d int,cpu_reset -serial stdio
```

---

## Boot Sequence

1. **BIOS** loads `boot.asm` at `0x7C00`.
2. Bootloader reads kernel sectors from disk into `0x1000`.
3. Sets VBE graphics mode `0x118` (1024×768, 32-bit).
4. Loads a minimal GDT and switches to 32-bit protected mode.
5. Jumps to `0x1000` → `kernel_entry.asm` → `kernel_main()`.

---

## Kernel Initialization Order (`kernel_main`)

| Step | Action |
|------|--------|
| 1 | `cli` — disable interrupts |
| 2 | `vga_clear()` — blank the screen |
| 3 | `pic_remap()` — remap PIC to IRQ 32–47 |
| 4 | `gdt_init()` — load GDT (kernel/user code+data, TSS) |
| 5 | `tss_init()` — set kernel stack for ring switches |
| 6 | `idt_init()` — register exception/IRQ/syscall handlers |
| 7 | `timer_init(100)` — configure PIT at 100 Hz |
| 8 | `task_init()` — set up two demo tasks |
| 9 | `paging_init()` — identity-map first 16 MB, enable CR0.PG |
| 10 | `heap_init()` — initialize bump allocator at 0x01000000 |
| 11 | `sti` — enable interrupts (timer starts driving scheduler) |
| 12 | `hlt` loop |

---

## Subsystem Overview

### VGA Text Output (`vga.c`)
- 80×25 colour text mode at `0xB8000`.
- `vga_print(str)` — prints a null-terminated string (supports `\n`).
- `vga_putchar(c)` — prints a single character.
- **Scrolling**: when the cursor reaches row 25 the screen is shifted up one line.

### Interrupt Handling (`idt.c`, `isr.asm`)
| Gate | Handler |
|------|---------|
| 13 | `#GP` — general protection fault |
| 14 | `#PF` — page fault (displays fault address from CR2) |
| 32 | Timer IRQ → scheduler tick |
| 33 | Keyboard IRQ |
| 0x80 | Syscall |

### Task Scheduler (`schedular.c`, `task_switch.asm`)
- Round-robin: on each timer tick `schedular_tick()` advances `current_task` and calls `task_switch(esp, ebp, eip)`.
- Two demo tasks (`task1_entry` / `task2_entry`) alternately print `A` / `B`.

### Memory Management (`paging.c`, `heap.c`)
- **Paging**: 4 page tables identity-map the first 16 MB (kernel + stack space).
- **Heap**: bump allocator starting at 0x01000000 (16 MB), 1 MB in size.
  - `kmalloc(size)` — 8-byte aligned allocation.
  - `kfree(ptr)` — no-op (bump allocator; memory is not reclaimed).

### Syscall Interface (`syscall.c`)
User code calls `int 0x80` with:
- `eax` = syscall number (`0` = write character)
- `ebx` = argument

Currently implemented syscalls:

| Number | Name | Argument |
|--------|------|----------|
| 0 | write | `ebx` = character to print |

---

## Known Issues & Limitations

### Bugs Fixed in This Revision
- **VGA no scrolling** — screen now scrolls when the cursor reaches row 25.
- **VGA dual cursor** — removed the redundant `cursor` integer; unified on `cursor_x` / `cursor_y`.
- **Page fault handler had no fault address** — `isr.asm` now reads CR2 and passes it to `page_fault_handler`.
- **EFLAGS comment wrong** — `0x202` correctly sets IF=1 (interrupts enabled) in user mode.
- **Task output disabled** — `vga_print` calls in `tasks.c` re-enabled so task switching is visible.
- **Interrupts never re-enabled** — `sti` added after full kernel initialization so the timer fires.

### Remaining Limitations

| Area | Issue |
|------|-------|
| **Task switching** | Stack context is not fully saved; reliable only for cooperative/HLT-based tasks |
| **Memory isolation** | All tasks share the same identity-mapped page tables |
| **Heap** | Bump allocator — `kfree` is a no-op; heap will eventually exhaust |
| **Page fault** | No recovery — any page fault halts the system |
| **Keyboard** | No shift/ctrl/alt support; limited scancode map; no input buffer |
| **Framebuffer** | VBE mode set by bootloader but framebuffer is never mapped or used |
| **User mode** | `enter_user_mode()` is wired up but not called by default; user stack at 0xE00000 must be mapped |
| **Filesystem** | None |
| **Syscalls** | Only syscall 0 (write) implemented |
| **Scheduler name** | File/function was misspelled as `schedular`; now corrected to `scheduler` |

---

## Memory Map

| Address Range | Contents |
|---------------|----------|
| `0x7C00` | Bootloader |
| `0x1000` | Kernel image |
| `0x70000` | Task 2 stack |
| `0x80000` | Task 1 stack |
| `0x90000` | Kernel ISR stack (TSS.esp0) |
| `0xE00000` | User mode stack (if user mode enabled) |
| `0x01000000` | Kernel heap (1 MB) |

---

## License

Educational / open source. No warranty.
