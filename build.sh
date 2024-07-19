#!/bin/bash

set -e

# Define directories and files
BOOT_DIR="boot"
KERNEL_DIR="kernel"
ISO_DIR="iso"
ISO_BOOT_DIR="$ISO_DIR/boot"
GRUB_DIR="$ISO_BOOT_DIR/grub"
KERNEL_ENTRY="$BOOT_DIR/kernel_entry.asm"
KERNEL_SRC="$KERNEL_DIR/kernel.c"
ISR_STUBS_SRC="$BOOT_DIR/isr_stubs.asm"
GDT_SRC="$BOOT_DIR/gdt.asm"
IDT_SRC="$BOOT_DIR/idt.asm"
FS_SRC="$KERNEL_DIR/fs.c"
KERNEL_BIN="$ISO_BOOT_DIR/kernel.bin"
GRUB_CFG="grub.cfg"
ISO="myos.iso"

# Create necessary directories
mkdir -p $ISO_BOOT_DIR $GRUB_DIR

# Assemble the kernel entry point
echo "Assembling kernel entry..."
nasm -f elf64 $KERNEL_ENTRY -o kernel_entry.o

# Assemble ISR stubs
echo "Assembling ISR stubs..."
nasm -f elf64 $ISR_STUBS_SRC -o isr_stubs.o

# Assemble GDT
echo "Assembling GDT..."
nasm -f elf64 $GDT_SRC -o gdt.o

# Assemble IDT
echo "Assembling IDT..."
nasm -f elf64 $IDT_SRC -o idt.o

# Compile the kernel
echo "Compiling kernel..."
gcc -m64 -ffreestanding -c $KERNEL_SRC -o kernel.o
gcc -m64 -ffreestanding -c $KERNEL_DIR/keyboard.c -o keyboard.o
gcc -m64 -ffreestanding -c $KERNEL_DIR/screen.c -o screen.o
gcc -m64 -ffreestanding -c $KERNEL_DIR/isr.c -o isr.o
gcc -m64 -ffreestanding -c $KERNEL_DIR/pmm.c -o pmm.o
gcc -m64 -ffreestanding -c $KERNEL_DIR/heap.c -o heap.o
gcc -m64 -ffreestanding -c $KERNEL_DIR/paging.c -o paging.o
gcc -m64 -ffreestanding -c $FS_SRC -o fs.o

# Link the kernel
echo "Linking kernel..."
ld -n -o $KERNEL_BIN -Ttext 0x100000 kernel_entry.o isr_stubs.o gdt.o idt.o kernel.o keyboard.o screen.o isr.o pmm.o heap.o paging.o fs.o --oformat binary

# Copy GRUB configuration
echo "Copying GRUB configuration..."
cp $GRUB_CFG $GRUB_DIR/

# Create bootable ISO
echo "Creating ISO image..."
grub-mkrescue -o $ISO $ISO_DIR

# Clean up intermediate files
echo "Cleaning up..."
rm -f kernel_entry.o isr_stubs.o gdt.o idt.o kernel.o keyboard.o screen.o isr.o pmm.o heap.o paging.o fs.o

echo "Build complete. ISO image is $ISO"
