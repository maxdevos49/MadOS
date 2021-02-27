; ==================     Interrupts   ======================
; Disables all IRQ's and loads a empty IDT
; Input: void
; Output: void
; ==========================================================
SECTION .text
setup_interrupts:
    
    mov al, 0xff        ; Disable all IRQ's by masking both PICS
    out 0xa1, al        ; Mask Slave PIC 
    out 0x21, al        ; Mask Master PIC

    nop
    nop

    lidt [IDT]          ; Load a zero length IDT so any NMI causes a triple fault

    ret

SECTION .data

ALIGN 4
IDT:
    .length: dw 0
    .base: dd 0
