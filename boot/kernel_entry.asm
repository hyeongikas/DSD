BITS 64
GLOBAL start
EXTERN kernel_main

start:
    ; Clear interrupts
    cli

    ; Set up a minimal stack
    mov rsp, stack_top

    ; Call the C kernel_main function
    call kernel_main

hang:
    hlt
    jmp hang

section .bss
resb 8192 ; 8 KB stack
stack_top:
