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

SECTORS_PER_FAT_OFFSET equ 0x24
; =======================================================================

SECTION .text
init_fat32:
    push eax

    mov eax, [0x7c00 + ]        ; TODO
    mov [FAT.SECTOR_BASE], eax

    mov eax, [0x7c00 + SECTORS_PER_FAT]
    mov [FAT.SECTOR_COUNT], eax


    pop eax
    ret

SECTION .bss
FAT:
    .SECTOR_BASE:           resw 1
    .SECTOR_COUNT:          resd 1
    .SECTORS_PER_CLUSTER:   resw 1

DATA:
    .SECTOR_BASE:           resw 1
    .SECTOR_COUNT:          resd 1
    .TOTAL_CLUSTERS:        resd 1
    .ROOT_CLUSTER:          resd 1


; ==================  Reads a File from a FAT32 Filesystem  ====================
; Reads a file to a given location based on inputs
; Input:
; Output: 
; ==============================================================================
SECTION .text
read_fat32:
    
    ret