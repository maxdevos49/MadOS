MEMORY_REGION_COUNT:
    db 0
    GLOBAL MEMORY_REGION_COUNT

detect_memory:
    mov bx, DETECTING_MEMORY
    call print_string

    mov ax, 0
    mov es, ax
    mov di, 0x6000   ; Map table to 0x6000 in memory

    mov edx, 0x534d4150
    xor ebx, ebx

    .repeat:
        mov eax, 0xe820
        mov ecx, 24
        int 0x15

        cmp ebx, 0
        je .finished

        add di, 24
        inc byte [MEMORY_REGION_COUNT]
        jmp .repeat

    .finished:
    ret

DETECTING_MEMORY: db "Detecting memory regions", 0xd, 0xa,0
