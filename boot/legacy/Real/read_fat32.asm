[bits 16]
; ==================     Init Fat32 Interactions     ====================
; Initilizes reading from a fat32 filesystem by collecting and setting 
; constants used later on
; Input:
;   void
; Output: 
;   void
; =======================================================================
; Constants:
SECTORS_PER_CLUSTER equ 0x0d
SECTORS_RESERVED    equ 0x0e
FAT_COUNT           equ 0x10
SECTOR_TOTAL        equ 0x20
SECTORS_PER_FAT     equ 0x24
ROOT_CLUSTER_NUMBER equ 0x2C
; =======================================================================

SECTION .bss
SIG1:                       resd 1
DRIVE:
    .TOTAL_SECTORS:         resd 1
    .NUMBER:                resd 1
FAT:
    .SECTOR_BASE:           resd 1
    .SIZE:                  resd 1
    .SECTORS_PER_CLUSTER:   resd 1
    .TABLE_COUNT:           resd 1
SIG2:                       resd 1
DATA:
    .SECTOR_BASE:           resd 1
    .SIZE:                  resd 1
    .TOTAL_CLUSTERS:        resd 1
    .ROOT_CLUSTER:          resd 1
SIG3:                       resd 1
; =======================================================================


SECTION .text
init_fat32:
    push eax
    push edx
    push ebx

    xor eax, eax                                ; Upper EAX is not cleared when ax is written too
    mov ax, [0x7c00 + SECTORS_RESERVED]         ; Read WORD 
    mov [FAT.SECTOR_BASE], eax                  ; Save result as DWORD

    xor eax, eax
    mov eax, [0x7c00 + SECTORS_PER_FAT]
    mov [FAT.SIZE], eax

    xor eax, eax
    mov al, [0x7c00 + SECTORS_PER_CLUSTER]
    mov [FAT.SECTORS_PER_CLUSTER], eax

    xor eax, eax
    mov al, [0x7c00 + FAT_COUNT]
    mov [FAT.TABLE_COUNT], eax

    mov eax, [0x7c00 + SECTOR_TOTAL]
    mov [DRIVE.TOTAL_SECTORS], eax

    mov eax, [FAT.SIZE]                         ; Load FAT.SIZE
    mul dword [FAT.TABLE_COUNT]                 ; (FAT.SIZE * FAT.TABLE_COUNT)
    add eax, [FAT.SECTOR_BASE]                  ; (FAT.SIZE * FAT.TABLE_COUNT) + SECTOR_BASE
    mov [DATA.SECTOR_BASE], eax                 ; Save DATA.SECTOR_BASE

    mov eax, [DRIVE.TOTAL_SECTORS]
    sub ebx, eax                    ; TOTAL_SECTORS - ((FAT.SIZE * FAT.TABLE_COUNT) + SECTOR_BASE)
    mov [DATA.SIZE], eax                        ; Save DATA.SIZE
    
    xor edx, edx                                ; Clear Dividend
    div dword [FAT.SECTORS_PER_CLUSTER]         ; DATA.SIZE / FAT.SECTORS_PER_CLUSTER
    mov [DATA.TOTAL_CLUSTERS], eax              ; Save DATA.TOTAL_CLUSTERS

    mov dword [SIG1], 0x44332211
    mov dword [SIG2], 0x88776655
    mov dword [SIG3], 0xccbbaa99

    pop ebx
    pop edx
    pop eax
    ret

; ==================  Reads a File from a FAT32 Filesystem  ====================
; Reads a file to a given location based on inputs
; Input:
; Output: 
; ==============================================================================
SECTION .text
read_fat32:
    
    ; Load root directory cluster
    ; ((cluster - 2) * FAT.SECTORS_PER_CLUSTER) + DATA.BASE)

    mov eax, [DATA.ROOT_CLUSTER]
    ; sub eax, 2 ; Seems to be wrong??
    mul dword [FAT.SECTORS_PER_CLUSTER]
    add eax, [DATA.SECTOR_BASE]

    push eax

    ; Load FAT
    xor ax, ax
    mov ds, ax
    mov di, 0x9000
    pop eax
    mov bx, 10
    call read_sectors


    DIR_ENTRY_FN equ 0
    DIR_ENTRY_EXT equ 8
    DIR_ENTRY_ATR equ 11
    DIR_ENTRY_LCLUS equ 20
    DIR_ENTRY_HCLUS equ 26
    DIR_ENTRY_SIZE equ 28
    .entry:

        mov al, byte [di + DIR_ENTRY_ATR]  ; Get Attribute Byte
        cmp al, 0x02            ; Entry is Hidden
        je .next
        cmp al, 0x04            ; Entry is a System File
        je .next
        cmp al, 0x08            ; Entry is the Volume Id
        je .next
        cmp al, 0x0f            ; Entry is a Long File Name
        je .next

        lea si, [di + DIR_ENTRY_FN]         ; Set source index to filename string
        mov ax, 8                           ; Set maximum string length
        mov bl, ' '                         ; Set optional delimiter for stopping
        call print_cd                       ; print_cd(si, ax=8, bl=' ')
        
        mov al, byte [di + DIR_ENTRY_ATR]   ; Load Directory Entry Attribute Byte
        cmp al, 0x10                        ; if(ATTRIBUTE == DIRECTORY) then
        je .skip_ext                        ; skip printing extension

        mov bx, DOT                         ; Set String pointer address
        call print                          ; print(bx)

        lea si, [di + DIR_ENTRY_EXT]        ; Set si to file extension string
        mov ax, 3                           ; Set maximum string length
        mov bl, ' '                         ; Set optional delimiter for stopping
        call print_cd                       ; print_cd(se, ax=3, bl=' ')

        .skip_ext:



        mov bx, NEW_LINE
        call print

        .next:

        add di, 32
        mov ebx, [es:di]

        cmp ebx, 0              ; if ([bx] != 0) then
        jne .entry              ; Print next entry

    ret

SECTION .data
NEW_LINE: db 0xA, 0xD, 0
DOT: db ".", 0

; ==================  Reads Sectors to a Location  ====================
; Reads some sectors to a given destination
; Input:
;   ds:di -> Destination
;   ax    -> Start Sector
;   bx    -> Count
; Output: 
;   void
; ==============================================================================
; https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH.3D42h:_Extended_Read_Sectors_From_Drive
SECTION .text
read_sectors:
    pusha

    mov byte [dap.size], 0x10           ; Size is always 0x10
    mov byte [dap.reserved], 0
    mov [dap.sectors_to_read], bx       ; Set how many sectors to read
    mov [dap.destination], di           ; Set Destination index
    mov [dap.destination + 2], ds       ; Set Destination segment
    mov [dap.lba_address], ax           ; Set Start Sector

    xor ax, ax
    mov ds, ax
    mov si, dap                         ; Set pointer to Disk address packet
    mov dl, 0x80                        ; Set first hard drive
    mov ah, 0x42                        ; BIOS Extended Read Sectors from Drive
  
    int 0x13                            ; Execute Function

    jc .error

    popa
    ret

    .error:
        mov bx, read_error_MSG
        call print
        jmp $

SECTION .data
read_error_MSG: db "Extended Read Failed", 0xA, 0xD, 0

SECTION .bss
dap:
    .size:              resb 1
    .reserved:          resb 1
    .sectors_to_read:   resw 1
    .destination:       resd 1
    .lba_address:       resq 1