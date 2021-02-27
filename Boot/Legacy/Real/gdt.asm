[bits 16]
; ==================     GDT   ======================
; Sets up a 64-bit GDT
; Input: void
; Output: void
; ==========================================================
; CONSTANTS
CODE_SEG equ 0x0008
DATA_SEG equ 0x0010
; ==========================================================
SECTION .data
GDT:
    .NULL:
        dq 0x0000000000000000
    .CODE:
        dq 0x00209A0000000000   ; 64-bit code descriptor (exec/read).
        dq 0x0000920000000000   ; 64-bit data descriptor (read/write).
 
ALIGN 4
    dw 0                              ; Padding to make the "address of the GDT" field aligned on a 4-byte boundary
 
.DESCRIPTOR:
    dw $ - GDT - 1                    ; 16-bit Size (Limit) of GDT.
    dd GDT                            ; 32-bit Base Address of GDT. (CPU will zero extend to 64-bit)

