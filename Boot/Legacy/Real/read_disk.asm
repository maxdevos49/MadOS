
; ==================     Read Disc    =======================
; Reads and loads memory from a disc drive to a given destination. Halts 
; if the read fails or the desired amount of sectors is not read
; Input: void
; Output: void
; ===========================================================
; Contants
SECTOR_COUNT equ 6  ; 60 or 64 Maximum
DESTINATION_POINTER equ 0x8000
;============================================================

[bits 16]
SECTION .text
read_disk:
    
    mov ah, 0x02                    ; BIOS read function
    mov bx, DESTINATION_POINTER     ; Memory location
    mov al, SECTOR_COUNT            ; Sectors to read
    mov dl, [DISC.boot_drive]       ; Which drive to use
    mov ch, 0x00                    ; Use Cylinder 0
    mov dh, 0x00                    ; Use Head 0
    mov cl, 0x02                    ; Start reading the second sector

    int 0x13                        ; Execute!

    jc .error                       ; if(carry bit == 1) then Read Failed

    cmp al, SECTOR_COUNT            ; if (al != SECTOR_COUNT) then
    jne .error_sector                      ; unexpected sector count error
    
    ret

    .error:                         ; Report General Error
        mov bx, .error_MSG
        call print
        jmp $

    .error_sector:                  ; Report Sector Count Error
        mov bx, .error_sector_MSG
        call print
        jmp $

; const variables
    .error_MSG: db "[ERROR] Disk Read Failed", 0
    .error_sector_MSG: db "[ERROR] Disk Read returned unexpected sector count", 0

DISC:
    .boot_drive: db 1
