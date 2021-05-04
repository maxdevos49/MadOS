[bits 16]
; ==================  Reads Sectors to a Location  ====================
; https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH.3D42h:_Extended_Read_Sectors_From_Drive
; Reads some sectors to a given destination
; Input:
;   fs:di -> Destination
;   eax    -> Start Sector
;   bx    -> Count
; Output:
;   void
; ==============================================================================
; Structures
struc dap, -16
    .size:              resb 1
    .reserved:          resb 1
    .sectors_to_read:   resw 1
    .destination:       resd 1
    .lba_lower:         resd 1
    .lba_upper:         resd 1
endstruc
; ==============================================================================
; read_sectors 0x9000=Destination, 50=Sector Start, 1=Count
%macro read_sectors 3
    push di
    push eax
    push bx
    mov di, %1
    mov eax, %2
    mov bx, %3
    call _read_sectors
    pop bx
    pop eax
    pop di
%endmacro
; ==============================================================================
SECTION .text
_read_sectors:
    push bp
    mov bp, sp
    sub sp, dap_size

    push eax
    push ds
    push si
    push dx

    mov byte [bp + dap.size], 0x10           ; Size is always 0x10
    mov byte [bp + dap.reserved], 0
    mov [bp + dap.sectors_to_read], bx       ; Set how many sectors to read
    mov [bp + dap.destination], di           ; Set Destination index
    mov [bp + dap.destination + 2], ds       ; Set Destination segment
    mov [bp + dap.lba_lower], eax            ; Set Start Sector
    mov dword [bp + dap.lba_upper], 0

    xor ax, ax
    mov ds, ax
    lea si, [bp - dap_size]                 ; Set pointer to Disk address packet
    mov dl, 0x80                            ; Set first hard drive
    mov ah, 0x42                            ; BIOS Extended Read Sectors from Drive
  
    int 0x13                                ; Execute BIOS Function

    jc .read_error

    pop dx
    pop si
    pop ds
    pop eax

    leave
    ret

    .read_error:
        puts READ_ERROR_MSG

        xchg al, ah
        call print_hex
        jmp $

SECTION .data
READ_ERROR_MSG: db "Extended Read Failed, Error: ", 0

