.intel_syntax noprefix
.code32

.set MAGIC, 0x1badb002
.set FLAGS, (1<<0 | 1<<1)
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
    .long MAGIC
    .long FLAGS
    .long CHECKSUM

.section .text
.extern kernelMain
.extern callConstructors
.global loader


loader:
    lea esp, [kernel_stack + 2*1024*1024]  # Use lea to get address of top of stack
    call callConstructors  # Call constructors before entering kernel main
    push eax
    push ebx
    call kernelMain

_stop:
    cli
    hlt
    jmp _stop

.section .bss
.align 16
kernel_stack:
    .space 2*1024*1024
