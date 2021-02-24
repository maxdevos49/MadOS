[bits 16]
[org 0x8000] ; TODO remove
; =====================   Extended Boot   =======================
; Begins with the assumption that long mode is supported 
; for the current machine. Sets up gdt, paging, and long
; mode for handing off to the kernel loader.
; 
; Input: void
; Output: void
;
; ===============================================================
; CONSTANTS

;===============================================================

jmp extended           ; Extended boot entry

%include "./Real/print.asm"

SECTION .text
extended:
    mov bx, extended_MSG
    call print

    jmp $

SECTION .rodata
extended_MSG: db "Extended half loaded", 0xA, 0xD, 0

times 2048-($-$$) db 0 ; Pad remaining 