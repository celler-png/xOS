/* =============================================================================
   xOS HARDWARE MATRIX - Master Linker Script (Android 12 Base Alignment)
   ============================================================================= */

ENTRY(_start)

SECTIONS
{
    /* Standard high-speed ARM64 mobile execution start address mapping position */
    . = 0x80000;

    .text : {
        /* Force your upgraded Secure Bootloader logic to sit at the absolute top */
        boot.o(.text)
        
        /* Auto-stamp the memory address tags your assembly checks depend on */
        secure_c_module_start = .;
        secure.o(.text)
        
        storage_files_module_start = .;
        Storagefiles.o(.text)
        
        touch_pipeline_module_start = .;
        touch_pipeline.o(.text)
        
        *(.text)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }
}
