BITS 64

extern isr_common_stub

%macro ISR 1
extern isr%1
isr%1:
    push qword %1
    jmp isr_common_stub
%endmacro

ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19
ISR 20
ISR 21
ISR 22
ISR 23
ISR 24
ISR 25
ISR 26
ISR 27
ISR 28
ISR 29
ISR 30
ISR 31

section .text
isr_common_stub:
    ; Save registers
    pusha
    push rsi
    push rdi
    push rdx
    push rcx
    push rbx
    push rax

    ; Call the ISR handler in C
    mov rdi, rsp
    call isr_handler

    ; Restore registers
    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rdi
    pop rsi
    popa
    add rsp, 8 ; Remove error code
    iretq
