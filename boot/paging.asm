BITS 64

align 4096
pml4_table:
    dq pml3_table | 0x3
    times 511 dq 0

align 4096
pml3_table:
    dq pml2_table | 0x3
    times 511 dq 0

align 4096
pml2_table:
    dq pml1_table | 0x3
    times 511 dq 0

align 4096
pml1_table:
    times 512 dq 0

section .text
extern load_paging
load_paging:
    mov rax, pml4_table
    mov cr3, rax
    mov rax, cr4
    or rax, 0x90
    mov cr4, rax
    mov rax, cr0
    or rax, 0x80000000
    mov cr0, rax
    ret
