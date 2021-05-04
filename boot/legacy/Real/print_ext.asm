[bits 16]
;====================== print_hex ===========================
; Prints a hex value pointed by ax
; Input: 
;   ax = Value
; Output:
;   void
;============================================================
; Macros
; print_hex value/16_bit register
%macro puth 1
    push ax
    mov ax, %1
    call print_hex
    pop ax
%endmacro
;============================================================
SECTION .text
print_hex:
    push bp
    mov bp, sp

    sub sp, 1               ; byte [bp - 1]  = uint8_t index
    sub sp, 9               ; byte [bp - 10] = char buffer[9]

    push ax
    push dx

    mov dx, ax

     ; First hex value
    push dx                 ; Store extra copy of bx
    shr dx, 12              ; shift dx 12 bits to the right
    mov bx, .HEX_TABLE      ; Store HEX_TABLE address onto cx
    add bx, dx              ; Calculate HEX_TABLE address
    mov al, [bx]            ; Dereference HEX Symbol and put in ax
    mov [bp-10], al         ; Store hex symbol in HEX_OUT
    pop dx                  ; Restore dx

    ; second hex value
    push dx
    shr dx, 8               ; shift dx 8 bits to the right
    and dx, 0x0f            ; remove bits at the beginning
    mov bx, .HEX_TABLE      ; Store HEX_TABLE address onto cx
    add bx, dx              ; Calculate HEX_TABLE address
    mov al, [bx]            ; Dereference HEX Symbol and put in ax
    mov [bp-10+ 1], al      ; Store hex symbol in HEX_OUT
    pop dx

    ; third hex value
    push dx
    shr dx, 4               ; shift dx 4 bits to the right
    and dx, 0x0f            ; Remove unneeded bits
    mov bx, .HEX_TABLE      ; Store HEX_TABLE address onto cx
    add bx, dx              ; Calculate HEX_TABLE address
    mov al, [bx]            ; Dereference HEX Symbol and put in ax
    mov [bp-10 + 2], al     ; Store hex symbol in HEX_OUT
    pop dx

    ; fourth hex value
    and dx, 0x0f
    mov bx, .HEX_TABLE
    add bx, dx
    mov al, [bx]
    mov [bp-10+3], al

    mov byte [bp-10+4], 0

    lea bx, [bp - 10] 
    call print

    pop dx
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

    mov cx, ax               ; Set Counter

    mov ah, 0x0e
    .for:
        mov al, [ds:si]
        int 0x10

        cmp al, 0xa
        je .return

        .next:
        inc si
        loop .for

    .exit:

    pop cx
    pop ax
    ret

    .return:
        mov al, 0xd
        int 0x10
        jmp .next

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

; ===================== print_char =========================
; Prints a single character to the TTY device
; Input:
;   al = char
; Output:
;   void
; ==========================================================
; Macro
%macro putc 1
    push ax
    mov al, %1
    call print_char
    pop ax
%endmacro
; ==========================================================
SECTION .text
print_char:
    push ax

    mov ah, 0x0e
    int 0x10
    
    pop ax
    ret


%include "./Real/print.asm"