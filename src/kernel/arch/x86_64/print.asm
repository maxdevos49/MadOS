; Prints a string refrenced by bx
print_string:
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

    pop ax
    pop bx
    ret