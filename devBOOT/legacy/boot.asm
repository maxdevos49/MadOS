[bits 16]
[org 0x7c00]

SECTION .text
jmp 0x0000:main                 ; Program Entry

%include "./Real/print.asm"
%include "./Real/read_disk.asm"
%include "./Real/cpuid.asm"

SECTION .text
main:
    cli                         ; Disable interrupts to avoid any unknowns
    
    xor ax, ax                  ; Set segment registers to known a known value (0)
    mov ds, ax                  ; "           "
    mov es, ax                  ; "           "
    mov ss, ax                  ; "           "

    mov [DISC.boot_drive], dl   ; Store boot drive number
    
    mov bp, 0x7c00              ; Set the stack base pointer
    mov sp, bp                  ; Set the stack pointer to the base pointer

    mov ah, 0x00                ; Mimic "clear" by resetting the text mode
    mov al, 0x03                ; Select text mode 80x25 16 colors
    int 0x10                    ; Activate

    mov bx, BOOT_MSG           ; Indicate we are in fact booting
    call print                  ; "           "

    in al, 0x92                 ; Enable A20. Allows for extended memory access up to 16MB in real mode
    or al, 2                    ; "           "
    out 0x92, al                ; "           "

    call check_cpuid            ; Check the cpu for long mode support
    call read_disk              ; Load the rest of the loader

    jmp DESTINATION_POINTER     ; Jump to extended half


BOOT_MSG: db "Legacy Loader booting...", 0xd, 0xa, 0

times 446-($-$$) db 0           ; Boot sector padding
db 0x80                         ; Byte indicates that this is a bootable partition
times 510-($-$$) db 0           ; Some more boot sector padding
dw 0xaa55                       ; Magic number indicating this is a boot sector