BITS 64

; GDT entry structure
struc gdt_entry
    .limit_low   resw 1
    .base_low    resw 1
    .base_middle resb 1
    .access      resb 1
    .granularity resb 1
    .base_high   resb 1
endstruc

align 16
gdt_start:
    resb 1
    resw 0xffff ; null descriptor

    resw 0xffff ; code segment
    resw 0
    resb 0
    resb 0x9a
    resb 0xaf
    resb 0

    resw 0xffff ; data segment
    resw 0
    resb 0
    resb 0x92
    resb 0xcf
    resb 0

gdt_end:

gdt_pointer:
    dw gdt_end - gdt_start - 1
    dd gdt_start

; Load GDT
extern load_gdt
load_gdt:
    lgdt [gdt_pointer]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush
flush:
    ret
