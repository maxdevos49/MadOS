[bits 16]

;=============== ax strcmp(es:si=str1, es:di=str2) ===============
; Compares two strings until one differes or a null character 
; is reached
; INPUT:
;   es:si = string 1
;   es:di = string 2
; OUTPUT:
;   ax = 0 means equal or 1 equals different
;==================
SECTION .text
strcmp:
    push bx
    push si
    push di

    .while:
        mov al, byte [es:si]
        mov bl, byte [es:di]
        
        cmp al, bl      ; if(al != bl)
        jne .not_equal  ; then break

        cmp al, 0       ; if(al == '\0')
        je .equal       ; then break

        inc si
        inc di
        jmp .while
    .end_while:
    
    .equal:
    pop di
    pop si
    pop bx
    mov ax, 0
    ret

    .not_equal:
    pop di
    pop si
    pop bx
    mov ax, 1
    ret