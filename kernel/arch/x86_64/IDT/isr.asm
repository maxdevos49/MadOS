GLOBAL _isr0
GLOBAL _isr1
GLOBAL _isr2
GLOBAL _isr3
GLOBAL _isr4
GLOBAL _isr5
GLOBAL _isr6
GLOBAL _isr7
GLOBAL _isr8
GLOBAL _isr9
GLOBAL _isr10
GLOBAL _isr11
GLOBAL _isr12
GLOBAL _isr13
GLOBAL _isr14
GLOBAL _isr15
GLOBAL _isr16
GLOBAL _isr17
GLOBAL _isr18
GLOBAL _isr19
GLOBAL _isr20
GLOBAL _isr21
GLOBAL _isr22
GLOBAL _isr23
GLOBAL _isr24
GLOBAL _isr25
GLOBAL _isr26
GLOBAL _isr27
GLOBAL _isr28
GLOBAL _isr29
GLOBAL _isr30
GLOBAL _isr31

; 0: Divide By Zero Exception
_isr0:
    cli
    push byte 0
    push byte 0
    jmp isr_common_stub

; 1: Debug Exception
_isr1:
    cli
    push byte 0
    push byte 1
    jmp isr_common_stub

; 2 : Non Maskable Interrupt Exception
_isr2:
    cli
    push byte 0
    push byte 2
    jmp isr_common_stub

; 3: Breakpoint Exception
_isr3:
    cli
    push byte 0
    push byte 3
    jmp isr_common_stub

; 4: Into Detected Overflow Exception
_isr4:
    cli
    push byte 0
    push byte 4
    jmp isr_common_stub

; 5: Out of Bounds Exception
_isr5:
    cli
    push byte 0
    push byte 5
    jmp isr_common_stub

; 6: Invalid Opcode Exception
_isr6:
    cli
    push byte 0
    push byte 6
    jmp isr_common_stub

; 7: No Coprocessor Exception
_isr7:
    cli
    push byte 0
    push byte 7
    jmp isr_common_stub

; 8: Double Fault Exception
_isr8:
    cli
    push byte 8
    jmp isr_common_stub

; 9: Coprocessor Segment Overrun Exception
_isr9:
    cli
    push byte 0
    push byte 9
    jmp isr_common_stub

; 10: Bad TSS Exception
_isr10:
    cli
    push byte 10
    jmp isr_common_stub

; 11: Segment Not Present Exception
_isr11:
    cli
    push byte 11
    jmp isr_common_stub

; 12: Stack Fault Exception
_isr12:
    cli
    push byte 12
    jmp isr_common_stub


; 13: General Protection Fault Exception
_isr13:
    cli
    push byte 13
    jmp isr_common_stub


; 14: Page Fault Exception
_isr14:
    cli
    push byte 0
    push byte 14
    jmp isr_common_stub

; 15: Unknown Interrupt Exception
_isr15:
    cli
    push byte 0
    push byte 15
    jmp isr_common_stub

; 16: Coprocessor Fault Exception
_isr16:
    cli
    push byte 0
    push byte 16
    jmp isr_common_stub

; 17: Alignment Check Exception (486+)
_isr17:
    cli
    push byte 0
    push byte 17
    jmp isr_common_stub

; 18: Machine Check Exception (Pentium/586+)
_isr18:
    cli
    push byte 0
    push byte 18
    jmp isr_common_stub

; 19: Reserved Exception
_isr19:
    cli
    push byte 0
    push byte 19
    jmp isr_common_stub

; 20: Reserved Exception
_isr20:
    cli
    push byte 0
    push byte 20
    jmp isr_common_stub

; 21: Reserved Exception
_isr21:
    cli
    push byte 0
    push byte 21
    jmp isr_common_stub

; 22: Reserved Exception
_isr22:
    cli
    push byte 0
    push byte 22
    jmp isr_common_stub

; 23: Reserved Exception
_isr23:
    cli
    push byte 0
    push byte 23
    jmp isr_common_stub

; 24: Reserved Exception
_isr24:
    cli
    push byte 0
    push byte 24
    jmp isr_common_stub

; 25: Reserved Exception
_isr25:
    cli
    push byte 0
    push byte 25
    jmp isr_common_stub

; 26: Reserved Exception
_isr26:
    cli
    push byte 0
    push byte 26
    jmp isr_common_stub

; 27: Reserved Exception
_isr27:
    cli
    push byte 0
    push byte 27
    jmp isr_common_stub

; 28: Reserved Exception
_isr28:
    cli
    push byte 0
    push byte 28
    jmp isr_common_stub

; 29: Reserved Exception
_isr29:
    cli
    push byte 0
    push byte 29
    jmp isr_common_stub

; 30: Reserved Exception
_isr30:
    cli
    push byte 0
    push byte 30
    jmp isr_common_stub

; 31: Reserved Exception
_isr31:
    cli
    push byte 0
    push byte 31
    jmp isr_common_stub


extern _fault_handler

isr_common_stub:
    PUSHALL
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10 ; Load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov rax,rsp ; Push us the stack
    push rax;
    mov rax, _fault_handler
    call rax    ; A special call, preserves the 'rip' register
    pop rax
    pop gs
    pop fs
    pop es
    pop ds
    POPALL
    add rsp, 16
    iret


; In 64 bit protected mode we do not have the instructions
; pusha or popa so we must define macros
%macro PUSHALL 0
    push rax
    push rcx
    push rdx
    push r8
    push r9
    push r10
    push r11
%endmacro

%macro POPALL 0
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdx
    pop rcx
    pop rax

%endmacro