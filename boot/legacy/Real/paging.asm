[bits 16]
; ==================     Paging   ======================
; Sets up and Identity Maps the first 2 MB of ram
; Input: void
; Output: void
; ==========================================================
; CONSTANTS
PML4 equ 0x1000
PDPT equ 0x2000
PDT equ 0x3000
PT equ 0x4000
; ==========================================================
SECTION .text
setup_paging:

    mov edi, PML4                   ; Point the destination index to the begining of the page tables
    mov cr3, edi                    ; Set the Control Register 3 to the destination index
    xor eax, eax                    ; Change EAX to 0
    mov ecx, 4096                   ; Set how many times to clear (4 * 4096) bytes
    rep stosd                       ; Clear the memory. Like memset

    mov edi, PML4                   ; Set edi to PML4 table again
    mov DWORD [edi], PDPT | 0x0003  ; Point to the first Page Directory Pointer Table(PDPT) entry

    mov edi, PDPT                   ; Move to the PDPT table
    mov DWORD [edi], PDT | 0x0003   ; Point to the first Page Directory Table(PDT)

    mov edi, PDT                    ; Move to the PDT table
    mov DWORD [edi], PT | 0x0003    ; Point to the first entry in the Page Table(PT)

    mov edi, PT                     ; Move to the PT table
    mov ebx, 0x00000003             ; Set the Page Table Entry template value
    mov ecx, 512                    ; Set how many entries to loop through and add

    .set_table_entry:               ; Fills the Page Table for identity mapping the first 2 MB
        mov DWORD [edi], ebx        ; Set the entry to the current entry represented in ebx
        add ebx, 0x1000             ; Add 0x1000 to ebx to increment the template value
        add edi, 8                  ; Move to the next entry
        loop .set_table_entry       ; Loop around until ecx is 0

    mov eax, cr4                    ; Retreive Control Register 4 Contents
    or eax, 1 << 5                  ; Enable the PAE bit
    or eax, 1 << 7                  ; Enable the PGE bit
    mov cr4, eax                    ; Apply the changes to CR4

    mov edx, PML4                   ; Move PML4 address into edx because CR3 cannot be directly accessed
    mov cr3, edx                    ; Set CR3 to point to our PML4 address

    ret