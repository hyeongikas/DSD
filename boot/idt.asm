BITS 64

extern keyboard_isr_handler

; IDT entry structure
struc idt_entry
    .offset_low   resw 1
    .selector     resw 1
    .ist          resb 1
    .type_attr    resb 1
    .offset_mid   resw 1
    .offset_high  resd 1
    .zero         resd 1
endstruc

align 16
idt_start:
    resb 1
    idt_entry isr0, 0x08, 0x8E
    idt_entry isr1, 0x08, 0x8E
    idt_entry isr2, 0x08, 0x8E
    idt_entry isr3, 0x08, 0x8E
    idt_entry isr4, 0x08, 0x8E
    idt_entry isr5, 0x08, 0x8E
    idt_entry isr6, 0x08, 0x8E
    idt_entry isr7, 0x08, 0x8E
    idt_entry isr8, 0x08, 0x8E
    idt_entry isr9, 0x08, 0x8E
    idt_entry isr10, 0x08, 0x8E
    idt_entry isr11, 0x08, 0x8E
    idt_entry isr12, 0x08, 0x8E
    idt_entry isr13, 0x08, 0x8E
    idt_entry isr14, 0x08, 0x8E
    idt_entry isr15, 0x08, 0x8E
    idt_entry isr16, 0x08, 0x8E
    idt_entry isr17, 0x08, 0x8E
    idt_entry isr18, 0x08, 0x8E
    idt_entry isr19, 0x08, 0x8E
    idt_entry isr20, 0x08, 0x8E
    idt_entry isr21, 0x08, 0x8E
    idt_entry isr22, 0x08, 0x8E
    idt_entry isr23, 0x08, 0x8E
    idt_entry isr24, 0x08, 0x8E
    idt_entry isr25, 0x08, 0x8E
    idt_entry isr26, 0x08, 0x8E
    idt_entry isr27, 0x08, 0x8E
    idt_entry isr28, 0x08, 0x8E
    idt_entry isr29, 0x08, 0x8E
    idt_entry isr30, 0x08, 0x8E
    idt_entry isr31, 0x08, 0x8E
    idt_entry keyboard_isr_handler, 0x08, 0x8E
idt_end:

idt_pointer:
    dw idt_end - idt_start - 1
    dq idt_start

; Load IDT
extern load_idt
load_idt:
    lidt [idt_pointer]
    ret
