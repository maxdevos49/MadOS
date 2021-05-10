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
; Fat 32 Directory Entry Struct
struc fat32_dir_entry
    .filename               resb 8
    .extension              resb 3
    .attributes             resb 1
    .reserved               resb 1
    .created_tenths_second  resb 1
    .created_time           resw 1
    .created_date           resw 1
    .accessed_date          resw 1
    .upper_cluster_num      resw 1
    .modification_time      resw 1
    .modification_date      resw 1
    .lower_cluster_num      resw 1
    .file_size              resd 1
endstruc

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

; ======================= read_cluster ======================
; Reads a cluster
; Input:
;   fs:di = destination pointer
;   eax = cluster to read
; Output:
;   eax = next cluster to read
; ===========================================================
; Macro
%macro read_cluster 2
    push di
    mov di, %1
    mov eax, %2
    call _read_cluster
    pop di
%endmacro
; ===========================================================
SECTION .text
_read_cluster:
    push bp
    mov bp, sp

    sub sp, 4               ; dword [bp - 4] = uint32_t next_cluster_num
    mov dword [bp - 4], 0   ; next_cluster_num = 0

    push eax
    push ebx
    push edx

    push eax

    mov ebx, 4
    mul ebx                 ; eax * 4
    
    xor edx, edx
    mov ebx, 512
    div ebx                 ; edx:eax / 512 = eax(fat_sector_index)
    
    add eax, [FAT.SECTOR_BASE]

    read_sectors di, eax, 1

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
    mov [bp - 4], eax       ; next_cluster_num = ((uint32_t*)destination)[cluster_num%128]

    pop eax
    xor edx, edx

    cmp eax, 0              ; Root directory does not follow the same rule??
    je .skip                
    sub eax, 2
    .skip:
    mul dword [FAT.SECTORS_PER_CLUSTER]
    add eax, [DATA.SECTOR_BASE]
    mov bx, 1
    
    read_sectors di, eax, 1

    pop edx
    pop ebx
    pop eax

    mov eax, [bp - 4]
    leave
    ret


; ==================  Get cluster Number  ====================
; Gets the cluster_number pointed by a given path recursively
; Input:
;   ds:si = String Path null or '/' terminated
;   fs:di = buffer atleast one cluster in length
;   eax   = Current Root Cluster Number
; Output: 
;   eax = Returned cluster number
;   edx = 
;   bx = Error code(0 = Success, 1 = Invalid Path, 2 = File not Found )
; Local Variables:
;   word [bp - 2]   = uint16_t i
;   word [bp - 15]  = char     file_name[13]
;   dword [bp - 19] = uint32_t relative_cluster_num
; =============================================================
; Macro
%macro get_cluster_number 3
    push si
    push di

    mov si, %1
    mov di, %2
    mov eax, %3
    call _get_cluster_number

    pop di
    pop si
%endmacro
; =============================================================
; Local variables struct
struc local_gcn, -32
    gcn_i                  resw 1   ; uint16_t i
    gcn_relative_cluster   resd 1   ; uint32_t relative_cluster_num
    gcn_filename           resb 13  ; char file_name[13]
    gcn_test_filename      resb 13  ; char test_file_name[13]
endstruc
; =============================================================
SECTION .text
_get_cluster_number:
    push bp
    mov bp, sp

    sub sp, local_gcn_size

    push si
    push di
    push cx

    mov word [bp + gcn_i], 0                    ; i = 0
    mov [bp + gcn_relative_cluster], eax         ; relative_cluster_num = eax

    cmp byte [ds:si], 0x2f                      ; if(path[0] == '\') 
    je .end_if                                  ; Then Skip if body
        mov bx, 1                               ; Set error code invalid path
        jmp .return                             ; Return
    .end_if:

    inc word [bp + gcn_i]                       ; i++

    .while:
        mov bx, word [bp + gcn_i]               ; bx = i
        mov al, byte [ds:si + bx]               ; al = path[i]

        cmp al, 0x2f                            ; if('/' == bl)
        je .end_while                           ; Then break
        cmp al, 0                               ; if('\0' == bl)
        je .end_while                           ; Then break

        add bx, bp
        mov byte [bx + gcn_filename - 1], al    ; file_name[i-1] = al

        inc word [bp + gcn_i]                   ; i++

        jmp .while
    .end_while:

    mov bx, word [bp + gcn_i]
    add bx, bp
    mov byte [bx + gcn_filename - 1], 0             ; file_name[i] = '\0'

    read_cluster di, dword [bp + gcn_relative_cluster]

    push di

    mov cx, 128                                 ; Maximum entries in 512 byte sector clusters
    .while_test_name:
        cmp byte[fs:di], 0                      ; if (entry.present == false)
        je .end_while_test_name                 ; break

        push si
        push di

        mov si, di
        lea di, [bp + gcn_test_filename]
        call _dir_entry_name                     ; dir_entry_name(dir_entry, filename_buffer)

        lea si, [bp + gcn_filename]
        lea di, [bp + gcn_test_filename]
        call strcmp

        pop di
        pop si

        cmp ax, 0
        je .match

        add di, 32                              ; Each entry is 32 bytes

        loop .while_test_name

    .match:
    xor eax, eax
    mov ax, [di + fat32_dir_entry.upper_cluster_num]
    rol eax, 16
    mov ax, [di + fat32_dir_entry.lower_cluster_num]

    mov bl, [di + fat32_dir_entry.attributes]

    ; mov edx, [di + fat32_dir_entry.file_size]

    ; puth dx
    ; rol edx, 16
    ; puth dx

    ; mov [bp + gcn_file_size], edx

    pop di

    test bl, 0x10
    jz .return

    mov dx, [bp + gcn_i]
    add si, dx
    get_cluster_number si, di, eax

    ; mov edx, [bp + gcn_file_size]           ; Return value
    ; puth dx
    mov bx, 0
    jmp .return

    .end_while_test_name:
    pop di              ; No matches!
    mov bx, 2           ; File not found
    
    .return:
    pop cx
    pop di
    pop si

    leave
    ret




;========= void dir_entry_name(fs:si=dir_entry, const es:di=buffer) ==========
; Loads a null terminated filename entry from a fat32 entry
; Input:
;   fs:si=dir_entry
;   es:di=filename_buffer
; Output:
;   void
;=======================================================================
SECTION .text
_dir_entry_name:
    push bp
    mov bp, sp

    sub sp, 1                   ; byte [bp - 1] = uint8_t entry_attributes

    push ax
    push cx
    push si
    push di

    cmp byte [fs:si], 0         ; if (entry.present == 0)
    je .return                 ; then return
    
    xor ax, ax
    mov al, byte [fs:si + fat32_dir_entry.attributes]
    mov [bp - 1], al            ; entry_attributes = entry.attributes

    mov edx, [fs:si + fat32_dir_entry.file_size]

    push si
    mov cx, 8
    .while_filename:
        mov al, byte [fs:si]
        cmp al, 0x20            ; if (char == ' ') then
        je .end_while_filename  ; break

        mov [es:di], al

        inc di
        inc si

        loop .while_filename
    .end_while_filename:
    pop si

    test byte [bp - 1], 0x20
    jz .return

    mov byte [es:di], 0x2e      ; File dot
    inc di

    add si, fat32_dir_entry.extension

    mov cx, 3
    .while_ext:
        mov al, byte [fs:si]

        cmp al, 0x20
        je .end_while_ext

        mov [es:di], al

        inc di
        inc si

        loop .while_ext
    .end_while_ext:

    .return:
    mov byte [es:di], 0

    pop di
    pop si
    pop cx
    pop ax
    
    leave
    ret
    

; ==================  Reads a File from a FAT32 Filesystem  ====================
; Reads a file to a given location based on inputs
; Input:
;   ds:si = String Path null terminated. Directories split via a '/'
;   fs:di = File data destination pointer
; Output: 
;   eax = bytes read
; ==============================================================================
; Macros
%macro read_fat32 2
    push si
    push di

    mov si, %1
    mov di, %2
    call _read_fat32

    pop di
    pop si
%endmacro
; ==============================================================================
SECTION .text
_read_fat32:
    push bp
    mov bp, sp

    push si
    push di
    push ebx
    push ecx
    push edx

    puts READ_FILE
    println si
    puts PROGRESS

    get_cluster_number si, di, 0

    push edx
    .while:
        read_cluster di, eax
        putc '.'

        cmp eax, 0
        je .corruption

        cmp eax, 0x0FFFFFF8
        jge .end_while

        jmp .while
    .end_while:

    putc ']'
    puts NEW_LINE
    
    .return:
    pop eax

    push edx
    push ecx
    push ebx
    push di
    push si

    leave
    ret

    .corruption:
        println CORRUPTION
        mov eax, 0
        jmp .return


SECTION .rodata
CORRUPTION: db "Read Failed. File Corrupted.", 0
READ_FILE: db "Reading: ", 0
PROGRESS: db "Progress: [", 0