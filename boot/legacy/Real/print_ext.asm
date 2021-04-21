[bits 16]

; ==================     Print Count    ====================
; Prints a string pointed by address in si but stops after a given length
; Input:
;   ds:si = string pointer
;   ax = length
; Output: void
; =====================================================
SECTION .text
print_c:
    push ax; 4 LENGTH
    push cx

    mov cx, 1         ; Set Counter

    mov ah, 0x0e      ; BIOS teletype function
    .loop:
    cmp cx, word [bp + 4]   ; if(COUNTER > MAX_LENGTH) then
    jg .exit                ; jump .exit
        mov al, [ds:si]
        int 0x10
        inc si
        inc cx
        jmp .loop
    .exit:

    pop cx
    pop ax
    ret

; ==================     Print Count or Delimiter    ====================
; Prints a string pointed by address in bx but stops after a given length 
; or delimeter is reached
; Input:
;   ds:si = string pointer
;   ax = maximum length
;   bl = delimter
; Output: void
; =====================================================
SECTION .text
print_cd:
    push ax; 4 MAX_LENGTH
    push cx

    mov cx, 1         ; Set Counter

    mov ah, 0x0e      ; BIOS teletype function
    .loop:
    cmp cx, word [bp + 4]   ; if(COUNTER > MAX_LENGTH) then
    jg .exit                ; jump .exit
    cmp bl, byte [ds:si]       ; if(DELIMITER == CURRENT_CHAR) then
    je .exit                ; jump .exit
        mov al, [ds:si]
        int 0x10
        inc si
        inc cx
        jmp .loop
    .exit:

    pop cx
    pop ax
    ret

%include "./Real/print.asm"