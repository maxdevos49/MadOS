[bits 16]

SECTION .text
print_hex:
    push bp
    mov bp, sp

    sub sp, 9       ; [bp - 11] = char buffer[9]
    sub sp, 1       ; byte [bp - 12] = uint8_t index

    push ax
    mov dx, ax

     ; First hex value
    push dx                 ; Store extra copy of bx
    shr dx, 12              ; shift dx 12 bits to the right
    mov bx, .HEX_TABLE       ; Store HEX_TABLE address onto cx
    add bx, dx              ; Calculate HEX_TABLE address
    mov al, [bx]            ; Dereference HEX Symbol and put in ax
    mov [bp-11], al     ; Store hex symbol in HEX_OUT
    pop dx                  ; Restore dx

    ; second hex value
    push dx
    shr dx, 8               ; shift dx 8 bits to the right
    and dx, 0x0f            ; remove bits at the beginning
    mov bx, .HEX_TABLE       ; Store HEX_TABLE address onto cx
    add bx, dx              ; Calculate HEX_TABLE address
    mov al, [bx]            ; Dereference HEX Symbol and put in ax
    mov [bp-11 + 1], al     ; Store hex symbol in HEX_OUT
    pop dx

    ; third hex value
    push dx
    shr dx, 4               ; shift dx 4 bits to the right
    and dx, 0x0f            ; Remove unneeded bits
    mov bx, .HEX_TABLE       ; Store HEX_TABLE address onto cx
    add bx, dx              ; Calculate HEX_TABLE address
    mov al, [bx]            ; Dereference HEX Symbol and put in ax
    mov [bp-11 + 2], al     ; Store hex symbol in HEX_OUT
    pop dx

    ; fourth hex value
    and dx, 0x0f
    mov bx, .HEX_TABLE
    add bx, dx
    mov al, [bx]
    mov [bp-11+3], al

    mov byte [bp-11+4], 0

    lea bx, [bp - 11] 
    call print

    pop ax
    leave
    ret

SECTION .rodata
    .HEX_TABLE: db "0123456789abcdef",0

; ==================     Print Count    ====================
; Prints a string pointed by address in si but stops after a 
; given length.
; Input:
;   ds:si = string pointer
;   ax = max length
; Output: void
; ==========================================================
SECTION .text
print_c:
    push ax; 4 MAX_LENGTH
    push cx

    mov cx, 1               ; Set Counter

    mov ah, 0x0e
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
; =======================================================================
SECTION .text
print_cd:
    push bp
    mov bp, sp

    push ax; 4 MAX_LENGTH
    push cx

    mov cx, 1               ; Set Counter

    mov ah, 0x0e
    .loop:                  ; while(cx < ax && bl != [ds:si])
    cmp cx, word [bp + 4]   ; if(COUNTER > MAX_LENGTH) then
    jg .exit_loop                ; jump .exit
    cmp bl, byte [ds:si]    ; if(DELIMITER == CURRENT_CHAR) then
    je .exit_loop                ; jump .exit
        mov al, [ds:si]
        int 0x10
        inc si
        inc cx
        jmp .loop
    .exit_loop:

    pop cx
    pop ax
    
    leave
    ret

%include "./Real/print.asm"