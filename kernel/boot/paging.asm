PAGE_PML4_ENTRY equ 0x1000

setup_identity_paging:

    ; Indicate paging location
    mov edi, PAGE_PML4_ENTRY
    mov cr3, edi

    ; clear PML4 entries
    xor eax, eax
    mov ecx, 0x1000 ; how many times to xor
    rep stosd       ; repeat with 32-bit increments. 64-bit increments if stosq


    ; get PML4 base again
    mov edi, PAGE_PML4_ENTRY

    ; init PML4 entry
    mov dword [edi], 0x2003
    add edi, 0x1000

    ; init PDP entry
    mov dword [edi], 0x000000f3;Gb 1
    add edi, 0x8
    mov dword [edi], 0x400000f3;Gb 2
    add edi, 0x8
    mov dword [edi], 0x800000f3;Gb 3
    add edi, 0x8
    mov dword [edi], 0xc00000f3;Gb 4

    ; ; init PD entry
    ; mov dword [edi], 0x4003
    ; add edi, 0x8
    ; mov dword [edi], 0x5003

    ; add edi, 0x0ff8

    ; ; Initilize Page Table Entries.
    ; mov ebx, 0x00000003       ; attributes: supervisor, read/write, present
    ; mov ecx, 1024             ; Maps(Identity) 4MB total memory
    ; .set_directory_entry:
    ;     mov dword [edi], ebx  ; Set Attributes
    ;     add ebx, 0x1000       ; Next physical frame
    ;     add edi, 8            ; Next entry
    ;     loop .set_directory_entry

        ; Set bit 5 in cr4 to Enable PAE. MUST be enabled before enabling long mode
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax


    ; Enable Long mode now that pae paging is set up
    mov ecx, 0xc0000080
    rdmsr                     ; Uses ecx to pick a MSR register. This case 0xc0000080 means the EFER register
    or eax, 1 << 8            ; Enable Long mode
    wrmsr                     ; Write changes back to the EFER register

                              ; Actually Enable paging
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    ret