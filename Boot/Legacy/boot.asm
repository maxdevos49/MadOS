; Constants
PROGRAM_SPACE equ 0x7e00
SECTOR_COUNT equ 4
MEMORY_MAP_BASE equ 0x1000
WIDTH equ 1024;px
HEIGHT equ 768;px
BPP equ 32

SECTION .text
[org 0x7c00]
jmp 0x0000:main           ; Boot Entry

SECTION .bss
BOOT_DRIVE: resb 1
MEMORY_REGION_COUNT: resb 1
FONT: resb 4096
VBE_INFO: resb 512
VBE_MODE_INFO: resb 512

SECTION .text
MSG: db "Rippy rip",0
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
    xor ax, ax      ; Setup Segment offset
    mov es, ax      ; "              "
    mov di, FONT    ; Set Segment index

    push ds
    push es

    ;ask BIOS to return VGA bitmap fonts
    mov ax, 1130h   ; Select BIOS VGA function
    mov bh, 6       ; Select Bitmap font
    int 10h         ; Execute!

    ;copy charmap   
    push es
    pop ds
    pop es
    mov si, bp
    mov cx, 256*16/4
    rep movsd
    pop	ds
    ; ==================  End Load Font  =========================

    ; ==================    Load VESA VBE    =====================
    mov word [.width], WIDTH    ; Store arguments
    mov word [.height], HEIGHT
    mov byte [.bpp], BPP

    sti

    xor ax, ax          ; Setup segment
    mov es, ax          ; "                 "
    push es
    mov ax, 0x4F00      ; Select VBE info function
    mov di, VBE_INFO    ; Setup segment index
    int 0x10            ; Execute!
    pop es

    cmp ax, 0x4F        ; If ax != 0x4f(Vesa support) then
    jne boot_error      ; report error

    mov ax, word [VBE_INFO + 5]
    mov [.segment], ax           ; Record Segment

    mov ax, word[VBE_INFO + 7]
    mov [.offset], ax     ; Record Segment index

    mov ax, [.segment]
    mov fs, ax
    mov si, [.offset]

    ; ==================  End Load VESA VBE  =====================

    ; ==================    Load VESA Mode    ====================
    .find_vesa_mode:
        mov dx, [fs:si]
        add si, 2
        mov [.offset], si
        mov [.mode], dx
        mov ax, 0
        mov fs, ax

        cmp word [.mode], 0xffff ; if VBE_MODE == 0xffff(End of list marker) then
        je boot_error ; Boot Error

        push es
        mov ax, 0x4f01          ; Select VBE mode info
        mov cx, [.mode]      ; Mode to select
        mov di, VBE_MODE_INFO   ; Info destination
        int 0x10                ; Execute!
        pop es

         mov bx, MSG
    call print_string

        mov ax, [.width]
        cmp ax, [VBE_MODE_INFO + 15] ; if (WIDTH != VBE_MODE_INFO.width) then
        jne .next_vesa_mode          ; try next mode

        mov ax, [.height]
        cmp ax, [VBE_MODE_INFO + 17] ; if (HEIGHT != VBE_MODE_INFO.height) then
        jne .next_vesa_mode          ; try next mode

        mov ax, [.bpp]
        cmp ax, [VBE_MODE_INFO + 25] ; if (BPP != VBE_MODE_INFO.bpp) then   
        jne .next_vesa_mode          ; try next mode

        ; if we reach here then we found a valid mode
        push es             ; Some Video BIOS trash es
        mov ax, 0x4f02      ; Select BIOS VESA function
        mov bx, [.mode]  ; Select VESA mode to activate
        ; or bx, 0x4000       ; Enable LFB (Preserves frame buffer)
        mov di,0            ; Some BIOS require di to be 0
        int 0x10            ; Execute!
        pop es              ; Restore Extended Segment

        cmp ax, 0x4f        ; if (ax != 0x4f) then
        jne boot_error      ; vesa mode failed to activate

        clc

        jmp .vesa_finish

    .next_vesa_mode:
        mov ax, [.segment]
        mov fs, ax
        mov si, [.offset]
        jmp .find_vesa_mode

    .width:         dw 0
    .height:        dw 0
    .bpp:           db 0
    .segment:       dw 0
    .offset:        dw 0
    .mode:          dw 0

    .vesa_finish:

    ; ==================  End Load VESA Mode  ====================

    jmp $

boot_error:
    mov bx, MSG
    call print_string
    jmp $

; Prints a string
; bx 
print_string:
    push ax
    push bx
    mov ah, 0x0e      ; BIOS teletype function
    .loop:
    cmp [bx], byte 0  ; if ([bx] === 0)
    je .exit          ; then goto exit
        mov al, [bx]  ; move the value in the address of bx to al
        int 0x10      ; print the ascii code to screen
        inc bx        ; move bx to next address
        jmp .loop
    .exit:

    pop ax
    pop bx
    ret

; ==================    Boot Sector Padding    =====================
times 446-($-$$) db 0
; Byte Indicates to older computers that this is a bootable partision
db 0x80

times 510-($-$$) db 0
; Magic number indicating this is a boot sector
dw 0xaa55
; ==================   End Boot Sector Padding  =====================

times 4096-($-$$) db 0