// =============================================================
// xOS OPERATING SYSTEM - Core ARM64 (AArch64) Bootloader
// =============================================================
.global _start

.section .text
_start:
    // 1. Set up a secure stack pointer for the CPU
    ldr x0, =_stack_top
    mov sp, x0

    // 2. Clear out core system registers
    mov x0, #0
    mov x1, #0

    // 3. Jump straight into our heavy C++ Kernel Main logic
    bl kernel_main

    // 4. Safe infinite loop if kernel ever returns
halt:
    b halt

