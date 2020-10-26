PAGE_TABLE_ENTRY equ 0x1000

setup_identity_paging:

    mov edi, PAGE_TABLE_ENTRY
    mov cr3, edi

    xor eax, eax
    mov ecx, 4096
    rep stosd
    mov edi, cr3

    mov dword [edi], 0x2003
    add edi, 0x1000
    mov dword [edi], 0x3003
    add edi, 0x1000
    mov dword [edi], 0x4003
    add edi, 0x1000

    mov ebx, 0x00000003
    mov ecx, 512

    .set_entry:             ; sub function has a dot
        mov dword [edi], ebx
        add ebx, 0x1000
        add edi, 8
        loop .set_entry

    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    mov ecx, 0xc0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, cr0
    or eax, 1 << 31 | 1 << 0
    mov cr0, eax

    ret