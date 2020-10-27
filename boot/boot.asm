
; Program offset
[org 0x7c00]          ; Offsets our compiled memory addresses

jmp 0x0000:main       ; Far jump right away

main:

    ; Set segment registers to known values
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax


    ; Store boot drive number
    mov [BOOT_DRIVE], dl

    ; init stack
    mov bp, 0x7c00    ; If kernel gets too big then this gets clobbered and it wont likely make it to the extended program
    mov sp, bp
    
    call read_disk
    
    ; Print a test string
    mov bx, EXTENDED_STRING
    call print_string

    jmp PROGRAM_SPACE ; Go to the extended program

%include "./read_disk.asm"
%include "./print.asm"


EXTENDED_STRING: db "Jumping to extended program",0xA, 0xD,0

; Bootsector padding

times 446-($-$$) db 0
; Byte Indicates to older computers that this is a bootable partision
db 0x80

times 510-($-$$) db 0
; Magic number indicating this is a boot sector
dw 0xaa55