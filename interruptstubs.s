.set IRQ_BASE, 0x20

.section .text


.extern _ZN16InterruptManager15handleInterruptEjj

.macro HandleException num
.global _ZN16InterruptManager16handleException\num\()Ev
    movb $\num, (interruptnumber)
    jmp int_bottom
.endm


.macro HandleInterruptRequest num
.global _ZN16InterruptManager26handleInterruptRequest\num\()Ev
    movb $\num + IRQ_BASE, (interruptnumber)
    jmp int_bottom
.endm
HandleInterruptRequest 0x00
HandleInterruptRequest 0x01

.global int_bottom

int_bottom:
    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    push %esp
    push (interruptnumber)
    call _ZN16InterruptManager15handleInterruptEjj
    # addl $5, %esp  # Clean up stack after call
    movl %eax, %esp  # Restore esp from return value

    popl %gs
    popl %fs
    popl %es
    popl %ds    
    popa

    iret

.data
    interruptnumber: .byte 0
