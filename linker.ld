/* xOS Memory Linker Configuration Script - ARM64 Layout */
ENTRY(_start)

SECTIONS
{
    /* ARM64 kernels commonly load at the 2 Megabyte mark (0x80000) */
    . = 0x80000;

    .text : {
        *(.text)
    }

    .rodata : {
        *(.rodata)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(COMMON)
        *(.bss)
    }

    /* Set aside safe space for our system stack */
    . = ALIGN(8);
    . = . + 0x4000; /* 16 Kilobytes of stack space */
    _stack_top = .;
}
