[bits 16]
[org 0x7c00]

SECTION .text
jmp _start                      ; Jump over the Extended BIOS Parameter Block if needed

times 0x5a-($-$$) db 0          ; Extended BIOS Parameter Block Padding

%include "./Real/print.asm"
%include "./Real/cpuid.asm"
%include "./Real/read_extended.asm"

SECTION .text
_start:
    jmp 0000:main
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

    mov bx, BOOT_MSG            ; Indicate we are in fact booting
    call print                  ; "           "

    in al, 0x92                 ; Enable A20. Allows for extended memory access up to 16MB in real mode
    or al, 2                    ; "           "
    out 0x92, al                ; "           "

    call check_cpuid            ; Does the computer even support long mode??
    call read_disk              ; Load the rest of the loader if long mode is supported

    jmp 0000:0x8000             ; Jump to extended half

BOOT_MSG: db "Legacy booting...", 0xd, 0xa, 0

times 440-($-$$) db 0           ; Boot Code Padding
times 510-($-$$) db 0           ; Partition Table padding
dw 0xaa55                       ; Magic number indicating this is a boot sector