;
; Implementation for legacy PAE???
;

PAGE_DIRECTORY_PTR_ENTRY equ 0x1000

setup_identity_paging:

    ; Indicate paging location
    mov edi, PAGE_DIRECTORY_PTR_ENTRY
    mov cr3, edi

    ; clear Page directory pointer entries
    xor eax, eax
    mov ecx, 0x1000    ; how many times to xor
    rep stosd          ; repeat with 32-bit increments. 64-bit increments if stosq

    ; get page directory location again
    mov edi, PAGE_DIRECTORY_PTR_ENTRY

    ; init 3 page directory pointers
    mov dword [edi], 0x2003
    add edi, 0x1000

    mov dword [edi], 0x3003
    add edi, 0x1000

    mov dword [edi], 0x4003
    add edi, 0x1000

    ; Initilizing page directory entries.
    mov ebx, 0x00000003 ; attributes: supervisor, read/write, present
    mov ecx, 512 ; how many times to loop
    .set_directory_entry:
        mov dword [edi], ebx
        add ebx, 0x1000
        add edi, 8
        loop .set_directory_entry

    ; Set bit 5 in cr4 to Enable PAE. MUST be enabled before enabling long mode
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax



    ; Enable Long mode now that pae paging is set up
    mov ecx, 0xc0000080
    rdmsr           ; Uses ecx to pick a MSR register. This case 0xc0000080 means the EFER register
    or eax, 1 << 8  ; Enable Long mode
    wrmsr           ; Write changes back to the EFER register

    ; Actually Enable paging
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    
    ret