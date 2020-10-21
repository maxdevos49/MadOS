GLOBAL _irq0
GLOBAL _irq1
GLOBAL _irq2
GLOBAL _irq3
GLOBAL _irq4
GLOBAL _irq5
GLOBAL _irq6
GLOBAL _irq7
GLOBAL _irq8
GLOBAL _irq9
GLOBAL _irq10
GLOBAL _irq11
GLOBAL _irq12
GLOBAL _irq13
GLOBAL _irq14
GLOBAL _irq15

; 32: IRQ0
_irq0:
    cli
    push byte 0            ; Push a dummy error code to the stack for consistency
    push byte 32           ; Actual interrupt number
    jmp irq_common_stub    ; Common interrupt handler

                           ; 33: IRQ1
_irq1:
    cli
    push byte 0
    push byte 33
    jmp irq_common_stub

    ; 34: IRQ2
_irq2:
    cli
    push byte 0
    push byte 34
    jmp irq_common_stub

    ; 35: IRQ3
_irq3:
    cli
    push byte 0
    push byte 35
    jmp irq_common_stub

    ; 36: IRQ4
_irq4:
    cli
    push byte 0
    push byte 36
    jmp irq_common_stub

    ; 37: IRQ5
_irq5:
    cli
    push byte 0
    push byte 37
    jmp irq_common_stub

    ; 38: IRQ6
_irq6:
    cli
    push byte 0
    push byte 38
    jmp irq_common_stub

    ; 39: IRQ7
_irq7:
    cli
    push byte 0
    push byte 39
    jmp irq_common_stub

    ; 40: IRQ8
_irq8:
    cli
    push byte 0
    push byte 40
    jmp irq_common_stub

    ; 41: IRQ9
_irq9:
    cli
    push byte 0
    push byte 41
    jmp irq_common_stub

    ; 42: IRQ10
_irq10:
    cli
    push byte 0
    push byte 42
    jmp irq_common_stub

    ; 43: IRQ11
_irq11:
    cli
    push byte 0
    push byte 43
    jmp irq_common_stub

    ; 44: IRQ12
_irq12:
    cli
    push byte 0
    push byte 44
    jmp irq_common_stub

    ; 45: IRQ13
_irq13:
    cli
    push byte 0
    push byte 45
    jmp irq_common_stub

    ; 46: IRQ14
_irq814:
    cli
    push byte 0
    push byte 46
    jmp irq_common_stub

    ; 47: IRQ15
_irq15:
    cli
    push byte 0
    push byte 47
    jmp irq_common_stub

extern _irq_handler

; This is a stub for IRQ based ISRs. This calls
; '_irq_handler' in the C code
irq_common_stub:
    PUSHALL
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10           ; Why, Guess is to set registered to a known state
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov rax, rsp
    push rax
    mov rax, __irq_handler ; Why -> preserve eip register
    call rax
    pop rax
    pop gs
    pop fs
    pop es
    pop ds
    POPALL
    add rsp, 16            ; Why/16 for 64-bit
    iret



    ; In 64 bit protected mode we do not have the instructions
    ; pusha or popa so we must define macros
%macro PUSHALL 0
    push rax
    push rbx
    push rcx
    push rdx

    push rbp
    push rsi
    push rdi

    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro

%macro POPALL 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8

    pop rdi
    pop rsi
    pop rbp

    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro