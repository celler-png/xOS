.global _start

.extern _binary_Image_XOS_bin_start
.extern secure_c_module_start       
.extern storage_files_module_start   
.extern touch_pipeline_module_start  

_start:
    ldr x0, =0x40000000              
    mov sp, x0

    ldr x1, =secure_c_module_start
    cbz x1, system_security_panic    

    ldr x2, =storage_files_module_start
    cbz x2, system_security_panic    

    ldr x3, =touch_pipeline_module_start
    cbz x3, system_security_panic    

    ldr x4, =_binary_Image_XOS_bin_start
    br x4                            

system_security_panic:
    mov x0, #0xFF                    
    hlt #0                           

halt_system:
    b halt_system
