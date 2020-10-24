[bits 64]

%include "../idt/PUSHPOP.asm"

; 0: Divide By Zero Exception
GLOBAL isr0
isr0:
    push 0
    push 0
    jmp isr_common_stub

    ; 1: Debug Exception
GLOBAL isr1
isr1:
    push 0
    push 1
    jmp isr_common_stub

    ; 2 : Non Maskable Interrupt Exception
GLOBAL isr2
isr2:
    push 0
    push 2
    jmp isr_common_stub

    ; 3: Breakpoint Exception
GLOBAL isr3
isr3:
    push 0
    push 3
    jmp isr_common_stub

    ; 4: Into Detected Overflow Exception
GLOBAL isr4
isr4:
    push 0
    push 4
    jmp isr_common_stub

    ; 5: Out of Bounds Exception
GLOBAL isr5
isr5:
    push 0
    push 5
    jmp isr_common_stub

    ; 6: Invalid Opcode Exception
GLOBAL isr6
isr6:
    push 0
    push 6
    jmp isr_common_stub

    ; 7: No Coprocessor Exception
GLOBAL isr7
isr7:
    push 0
    push 7
    jmp isr_common_stub

    ; 8: Double Fault Exception
GLOBAL isr8
isr8:
    push 8
    jmp isr_common_stub

    ; 9: Coprocessor Segment Overrun Exception
GLOBAL isr9
isr9:
    push 0
    push 9
    jmp isr_common_stub

    ; 10: Bad TSS Exception
GLOBAL isr10
isr10:
    push 10
    jmp isr_common_stub

    ; 11: Segment Not Present Exception
GLOBAL isr11
isr11:
    push 11
    jmp isr_common_stub

    ; 12: Stack Fault Exception
GLOBAL isr12
isr12:
    push 12
    jmp isr_common_stub


    ; 13: General Protection Fault Exception
GLOBAL isr13
isr13:
    push 13
    jmp isr_common_stub

    ; 14: Page Fault Exception
GLOBAL isr14
isr14:
    ; push 0
    push 14
    jmp isr_common_stub

    ; 15: Unknown Interrupt Exception
GLOBAL isr15
isr15:
    push 0
    push 15
    jmp isr_common_stub

    ; 16: Coprocessor Fault Exception
GLOBAL isr16
isr16:
    push 0
    push 16
    jmp isr_common_stub

    ; 17: Alignment Check Exception (486+)
GLOBAL isr17
isr17:
    push 0
    push 17
    jmp isr_common_stub

    ; 18: Machine Check Exception (Pentium/586+)
GLOBAL isr18
isr18:
    push 0
    push 18
    jmp isr_common_stub

    ; 19: Reserved Exception
GLOBAL isr19
isr19:
    push 0
    push 19
    jmp isr_common_stub

    ; 20: Reserved Exception
GLOBAL isr20
isr20:
    push 0
    push 20
    jmp isr_common_stub

    ; 21: Reserved Exception
GLOBAL isr21
isr21:
    push 0
    push 21
    jmp isr_common_stub

    ; 22: Reserved Exception
GLOBAL isr22
isr22:
    push 0
    push 22
    jmp isr_common_stub

    ; 23: Reserved Exception
GLOBAL isr23
isr23:
    push 0
    push 23
    jmp isr_common_stub

    ; 24: Reserved Exception
GLOBAL isr24
isr24:
    push 0
    push 24
    jmp isr_common_stub

    ; 25: Reserved Exception
GLOBAL isr25
isr25:
    push 0
    push 25
    jmp isr_common_stub

    ; 26: Reserved Exception
GLOBAL isr26
isr26:
    push 0
    push 26
    jmp isr_common_stub

    ; 27: Reserved Exception
GLOBAL isr27
isr27:
    push 0
    push 27
    jmp isr_common_stub

    ; 28: Reserved Exception
GLOBAL isr28
isr28:
    push 0
    push 28
    jmp isr_common_stub

    ; 29: Reserved Exception
GLOBAL isr29
isr29:
    push 0
    push 29
    jmp isr_common_stub

    ; 30: Reserved Exception
GLOBAL isr30
isr30:
    push 0
    push 30
    jmp isr_common_stub

    ; 31: Reserved Exception
GLOBAL isr31
isr31:
    push 0
    push 31
    jmp isr_common_stub


[extern fault_handler]

isr_common_stub:
    PUSHALL
  
    mov rdi, rsp
    call fault_handler

    POPALL
    add rsp, 16
    iretq
