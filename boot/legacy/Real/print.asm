[bits 16]
; ==================     Print     ====================
; Prints a string pointed by address in bx
; Input:
;   bx = string pointer
; Output: void
; =====================================================
; MACROs:
%macro puts 1
    push bx
    mov bx, %1
    call print
    pop bx
%endmacro

%macro println 1
    push bx
    mov bx, %1
    call print
    mov bx, NEW_LINE
    call print
    pop bx
%endmacro
;======================================================
; Constants
NULL equ 0
;======================================================
; GLOBALS
SECTION .rodata
NEW_LINE: db 0xA, 0xD, 0
;======================================================
SECTION .text
print:
    push ax
    push bx

    mov ah, 0x0e      ; BIOS teletype function
    .loop:
    cmp [bx], byte 0  ; if ([bx] === 0)
    je .exit          ; then goto exit
        mov al, [bx]  ; move the value in the address of bx to al
        int 0x10      ; print the ascii code to screen
        inc bx        ; move bx to next address
        jmp .loop
    .exit:

    pop bx
    pop ax
    ret