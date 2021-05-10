[bits 16]
; ==================     Memory Map   ======================
; https://wiki.osdev.org/Detecting_Memory_(x86)#Detecting_Upper_Memory
; Retrieves the memory map and stores it at 0000:MEMORY_MAP_BASE. Also 
; sets the Total memory region count at 0000:MEMORY_REGION_COUNT.
; Input: void
; Output: void
; ==========================================================
; CONSTANTS
MEMORY_MAP_BASE equ 0x5000
; ==========================================================

SECTION .text
memmap:
    pusha
    xor ax, ax                   ; Set extended segment offset to 0x0000
    mov es, ax                  ; "    "
    mov di, MEMORY_MAP_BASE     ; Map table to MEMORY_MAP_BASE

    mov DWORD [MEMORYMAP_DESCRIPTOR.base], MEMORY_MAP_BASE ; Save Memort Map Base

    xor ebx, ebx                ; Zero ebx
    mov edx, 0x534D4150         ; Magic number
    xor eax, eax

    .repeat:
        mov eax, 0xe820         ; BIOS memmap function
        mov ecx, 24             ; Entry Size
        int 0x15                ; Call BIOS memmap function

        cmp ebx, 0              ; if (ebx == 0) then
        je .finished     ; end loop

        add di, 24                      ; Increment Destination Index
        inc byte [MEMORYMAP_DESCRIPTOR.size]  ; Increment Region count
        jmp .repeat              ; Wash, Rinse, and Repeat!

    .finished:
    popa
    ret

SECTION .bss
GLOBAL MEMORYMAP_DESCRIPTOR
MEMORYMAP_DESCRIPTOR:
    .size: resw 1
    .base: resd 1