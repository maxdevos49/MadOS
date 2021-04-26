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
;   ds:si = String Path null terminated. Directories split via a '/'
;   fs:di = File data destination pointer
; Output: 
;   dword [bp + 6] = Bytes Read
; Local Variables:
; ==============================================================================
SECTION .text
read_fat32:
    push bp
    mov bp, sp

    xor ax, ax
    mov ds, ax
    mov si, PATH
    mov eax, 0
    call get_cluster_number

    ; mov eax, [FAT.SECTOR_BASE]
    ; call print_hex

    leave
    ret

SECTION .rodata
PATH: db "/this/is/a/test.txt",0

; ==================  Get cluster Number  ====================
; Gets the cluster_number pointed by a given path recursively
; Input:
;   ds:si = String Path null or '/' terminated
;   eax   = Current Root Cluster Number
; Output: 
;   eax = Returned cluster number
;   bx = Error code(0 = Success, 1 = Invalid Path, )
; Local Variables:
;   word [bp - 2]   = uint16_t i
;   word [bp - 15]  = char     file_name[13]
;   dword [bp - 19] = uint32_t relative_cluster_num
; ==============================================================================
SECTION .text
get_cluster_number:
    push bp
    mov bp, sp

    sub sp, 2                       ; word [bp - 2]  i
    sub sp, 13                      ; byte [bp - 15 + i] file_name[13]
    sub sp, 4                       ; dword [bp - 19] relative_cluster_num
    mov word [bp - 2], 0            ; i = 0

    cmp byte [ds:si], 0x2f          ; if(path[0] == '\') 
    je .end_if                      ; Then Skip if body
        mov bx, 1                   ; Set error code invalid path
        jmp .return                 ; Return
    .end_if:

    inc word [bp - 2]               ; i++

    .while:
        mov bx, word [bp - 2]       ; bx = i
        mov al, byte [ds:si + bx]   ; al = path[i]

        cmp al, 0x2f                ; if('/' == bl)
        je .end_while               ; Then break
        cmp al, 0                   ; if('\0' == bl)
        je .end_while               ; Then break

        add bx, bp
        mov byte [bx - 15 - 1], al  ; file_name[i-1] = al

        inc word [bp - 2]           ; i++
        jmp .while
    .end_while:

    mov bx, word [bp - 2]
    add bx, bp
    mov byte [bx - 15], 0           ; file_name[i] = '\0'

    push ds
    xor ax, ax
    mov ds, ax
    mov di, 0x9000
    mov eax, 1
    call read_cluster       ; read_cluster(0x9000, cluster_num)
    pop ds

    .return:
    leave
    ret

; ======== read_cluster ========
; Reads a cluster
; Input:
;   ds:di = destination pointer
;   eax = cluster to read
; Output:
;   eax = next cluster to read
; Local Variables:
;   dword [bp - 6] = uint32_t next_cluster_num
; ====
SECTION .text
read_cluster:
    push bp
    mov bp, sp

    sub sp, 4               ; dword [bp - 6]

    push eax                ; Save copy of eax

    mov ebx, 4
    mul ebx                 ; eax * 4
    
    xor edx, edx
    mov ebx, 512
    div ebx                 ; edx:eax / 512
    
    add eax, [FAT.SECTOR_BASE] ; FAT_SECTOR_INDEX + FAT_SECTOR_BASE
    mov bx, 1
    call read_sectors       ; read_sectors(destination=ds:di, sector_base=ax, count=bx)
    
    pop eax
    push eax
    mov ebx, 128
    xor edx, edx
    div ebx
    mov eax, edx            ; eax % ebx == cluster_num % 128

    mov ebx, 4
    mul ebx
    mov ebx, eax
    mov eax, [di + bx]
    mov [bp - 6], eax       ; next_cluster_num = ((uint32_t*)destination)[cluster_num%128]

    pop eax
    xor edx, edx
    mul dword [FAT.SECTORS_PER_CLUSTER]
    add eax, [DATA.SECTOR_BASE]
    mov bx, 1
    call read_sectors                   ; read_sectors(destination=ds:di, sector_start:eax, count=1)

    mov eax, [bp - 6]
    leave
    ret

; ==================  Reads Sectors to a Location  ====================
; Reads some sectors to a given destination
; Input:
;   ds:di -> Destination
;   eax    -> Start Sector
;   bx    -> Count
; Output: 
; ==============================================================================
; https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH.3D42h:_Extended_Read_Sectors_From_Drive
SECTION .text
read_sectors:
    push bp
    mov bp, sp

    push eax
    push dx
    push ds
    push si

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

    pop si
    pop ds
    pop dx
    pop eax

    leave
    ret

    .error:
        mov bx, read_error_MSG
        call print
        xchg al, ah
        call print_hex
        jmp $

SECTION .data
read_error_MSG: db "Extended Read Failed, Error: ", 0

SECTION .bss
dap:
    .size:              resb 1
    .reserved:          resb 1
    .sectors_to_read:   resw 1
    .destination:       resd 1
    .lba_address:       resq 1


; SECTION .rodata
; EXPECTED: db "Expected: 'this', Full Path: ",0
    

    ; lea bx, [bp-15]             ; Testing so far
    ; call print                 ; print(file_name)
    
    ; mov bx, NEW_LINE
    ; call print

    ; mov bx, EXPECTED 
    ; call print

    ; mov bx, PATH 
    ; call print
    ; Load root directory cluster
    ; ((cluster - 2) * FAT.SECTORS_PER_CLUSTER) + DATA.BASE)

    ; mov eax, [DATA.ROOT_CLUSTER]
    ; ; sub eax, 2 ; Seems to be wrong??
    ; mul dword [FAT.SECTORS_PER_CLUSTER]
    ; add eax, [DATA.SECTOR_BASE]

    ; push eax

    ; ; Load FAT
    ; xor ax, ax
    ; mov ds, ax
    ; mov di, 0x9000
    ; pop eax
    ; mov bx, 10
    ; call read_sectors


    ; DIR_ENTRY_FN equ 0
    ; DIR_ENTRY_EXT equ 8
    ; DIR_ENTRY_ATR equ 11
    ; DIR_ENTRY_LCLUS equ 20
    ; DIR_ENTRY_HCLUS equ 26
    ; DIR_ENTRY_SIZE equ 28
    ; .entry:

    ;     mov al, byte [di + DIR_ENTRY_ATR]  ; Get Attribute Byte
    ;     cmp al, 0x02            ; Entry is Hidden
    ;     je .next
    ;     cmp al, 0x04            ; Entry is a System File
    ;     je .next
    ;     cmp al, 0x08            ; Entry is the Volume Id
    ;     je .next
    ;     cmp al, 0x0f            ; Entry is a Long File Name
    ;     je .next

    ;     lea si, [di + DIR_ENTRY_FN]         ; Set source index to filename string
    ;     mov ax, 8                           ; Set maximum string length
    ;     mov bl, ' '                         ; Set optional delimiter for stopping
    ;     call print_cd                       ; print_cd(si, ax=8, bl=' ')
        
    ;     mov al, byte [di + DIR_ENTRY_ATR]   ; Load Directory Entry Attribute Byte
    ;     cmp al, 0x10                        ; if(ATTRIBUTE == DIRECTORY) then
    ;     je .skip_ext                        ; skip printing extension

    ;     mov bx, DOT                         ; Set String pointer address
    ;     call print                          ; print(bx)

    ;     lea si, [di + DIR_ENTRY_EXT]        ; Set si to file extension string
    ;     mov ax, 3                           ; Set maximum string length
    ;     mov bl, ' '                         ; Set optional delimiter for stopping
    ;     call print_cd                       ; print_cd(se, ax=3, bl=' ')

    ;     .skip_ext:



    ;     mov bx, NEW_LINE
    ;     call print

    ;     .next:

    ;     add di, 32
    ;     mov ebx, [es:di]

    ;     cmp ebx, 0              ; if ([bx] != 0) then
    ;     jne .entry              ; Print next entry

    ; ret

SECTION .data
NEW_LINE: db 0xA, 0xD, 0
DOT: db ".", 0

