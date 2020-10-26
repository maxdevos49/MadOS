[bits 64]

%include "interrupts/PUSHPOP.asm"

; 32: IRQ0
GLOBAL irq0
irq0:
    push 0         ; push a dummy error code to the stack for consistency
    push 32        ; Actual interrupt number
    jmp irq_common_stub ; Common interrupt handler

                        ; 33: IRQ1
GLOBAL irq1
irq1:
    push 0
    push 33
    jmp irq_common_stub

    ; 34: IRQ2
GLOBAL irq2
irq2:
    push 0
    push 34
    jmp irq_common_stub

    ; 35: IRQ3
GLOBAL irq3
irq3:
    push 0
    push 35
    jmp irq_common_stub

    ; 36: IRQ4
GLOBAL irq4
irq4:
    push 0
    push 36
    jmp irq_common_stub

    ; 37: IRQ5
GLOBAL irq5
irq5:
    push 0
    push 37
    jmp irq_common_stub

    ; 38: IRQ6
GLOBAL irq6
irq6:
    push 0
    push 38
    jmp irq_common_stub

    ; 39: IRQ7
GLOBAL irq7
irq7:
    push 0
    push 39
    jmp irq_common_stub

    ; 40: IRQ8
GLOBAL irq8
irq8:
    push 0
    push 40
    jmp irq_common_stub

    ; 41: IRQ9
GLOBAL irq9
irq9:
    push 0
    push 41
    jmp irq_common_stub

    ; 42: IRQ10
GLOBAL irq10
irq10:
    push 0
    push 42
    jmp irq_common_stub

    ; 43: IRQ11
GLOBAL irq11
irq11:
    push 0
    push 43
    jmp irq_common_stub

    ; 44: IRQ12
GLOBAL irq12
irq12:
    push 0
    push 44
    jmp irq_common_stub

    ; 45: IRQ13
GLOBAL irq13
irq13:
    push 0
    push 45
    jmp irq_common_stub

    ; 46: IRQ14
GLOBAL irq14
irq14:
    push 0
    push 46
    jmp irq_common_stub

    ; 47: IRQ15
GLOBAL irq15
irq15:
    push 0
    push 47
    jmp irq_common_stub

[extern irq_handler]

; This is a stub for IRQ based ISRs. This calls
; 'irq_handler' in the C code
irq_common_stub:
    PUSHALL

    mov rdi, rsp
    call irq_handler

    POPALL
    add rsp, 16
    iretq