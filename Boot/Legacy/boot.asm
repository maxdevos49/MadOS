
PROGRAM_SPACE equ 0x7d00
SECTOR_COUNT equ 30
FONT_BASE equ 0x1000
MEMORY_MAP_BASE equ 0x2000

[org 0x7c00]
jmp 0x0000:main           ; Entry

BOOT_DRIVE: db 0
MEMORY_REGION_COUNT: db 0

main:
    ; Set segment registers to known values
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax

    ; Store boot drive number
    mov [BOOT_DRIVE], dl

    ; init stack
    mov bp, 0x7c00
    mov sp, bp

    ; ==================     Read Disc    =======================
    mov ah, 0x02          ; BIOS read function
    mov bx, PROGRAM_SPACE ; Memory location
    mov al, SECTOR_COUNT  ; Sectors to read
    mov dl, [BOOT_DRIVE]  ; Which drive to use
    mov ch, 0x00          ; Use Cylinder 0
    mov dh, 0x00          ; Use Head 0
    mov cl, 0x02          ; Start reading the second sector

    int 0x13              ; Do the read

    jc boot_error         ; Read Failed

    cmp al, SECTOR_COUNT  ; if (al != SECTOR_COUNT)
    jne boot_error        ; expected sector error
    ; ================== End Read Disc =========================

    ; ==================     Memory Map   ======================
    ; https://wiki.osdev.org/Detecting_Memory_(x86)#Detecting_Upper_Memory

    mov ax, 0                   ; Set extended segment offset to 0x0000
    mov es, ax                  ; "    "
    mov di, MEMORY_MAP_BASE     ; Map table to MEMORY_MAP_BASE

    mov edx, 0x534d4150         ; Magic number
    xor ebx, ebx                ; Zero ebx

    .memmap_repeat:
        mov eax, 0xe820         ; BIOS memmap function
        mov ecx, 24             ; Entry Size
        int 0x15                ; Call BIOS memmap function

        cmp ebx, 0              ; if (ebx == 0) then
        je .memmap_finished     ; end loop

        add di, 24                      ; Increment Destination Index
        inc byte [MEMORY_REGION_COUNT]  ; Increment Region count
        jmp .memmap_repeat              ; Wash, Rinse, and Repeat!

    .memmap_finished:
    ; ==================  End Memory Map  ======================

    ; ==================    Enable A20    ======================
    ; Allows for extended memory access up to 16MB in real mode
    in al, 0x92     
    or al, 2
    out 0x92, al
    ; ==================  End Enable A20  ========================

    ; ==================    Load Font    =========================
    ;TODO
    ; ==================  End Load Font  =========================

    ; ==================    Load VESA VBE    =====================
    ;TODO
    ; ==================  End Load VESA VBE  =====================


    jmp $


boot_error:
;TODO print error occured and instruction pointer
    hlt

; ==================    Boot Sector Padding    =====================
times 446-($-$$) db 0
; Byte Indicates to older computers that this is a bootable partision
db 0x80

times 510-($-$$) db 0
; Magic number indicating this is a boot sector
dw 0xaa55
; ==================   End Boot Sector Padding  =====================