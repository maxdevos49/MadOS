; Constants(Think like c macros)
PROGRAM_SPACE equ 0x8000  
SECTOR_COUNT equ 70

; Loads extended program
read_disk:
    mov ah, 0x02          ; BIOS read function
    mov bx, PROGRAM_SPACE ; Memory location
    mov al, SECTOR_COUNT  ; Sectors to read
    mov dl, [BOOT_DRIVE]  ; Which drive to use
    mov ch, 0x00          ; Use Cylinder 0
    mov dh, 0x00          ; Use Head 0
    mov cl, 0x02          ; Start reading the second sector

    int 0x13              ; Do the read

    jc disk_read_error    ; Read Failed

    cmp al, SECTOR_COUNT  ; if (al != 4)
    jne disk_sector_error ; then goto sector error

    ; Complete success
    mov bx, DISK_SUCCESS
    call print_string
    ret


disk_read_error:
    mov bx, DISK_READ_ERROR
    call print_string
    jmp $

disk_sector_error:
    mov bx, DISK_SECTOR_ERROR
    call print_string
    jmp $

    ; Global variables
BOOT_DRIVE:  db 0
DISK_READ_ERROR: db "Disk read failed", 0xA, 0xD, 0
DISK_SECTOR_ERROR: db "Unexpected amount of sectors returned", 0xA, 0xD, 0
DISK_SUCCESS: db "Successfully read disk", 0xA, 0xD, 0
