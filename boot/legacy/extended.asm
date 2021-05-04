[bits 16]
[org 0x8000]
SECTION .text
jmp extended

%include "./Real/string.asm"
%include "./Real/print_ext.asm"
%include "./Real/read_disk_ext.asm"
%include "./Real/read_fat32.asm"

SECTION .text
extended:

    println EXTENDED_MSG

    call init_fat32                     ; Initilizes details for reading from a fat32 filesystem

    xor ax, ax
    mov fs, ax
    mov es, ax
    mov ds, ax

    read_fat32 PATH, 0x9000

    cmp bx, 0
    jne .read_error

    mov si, 0x9000
    call print_c

    jmp $

    .read_error
        puts INVALID_PATH
        puth bx
        jmp $
    
SECTION .rodata
PATH: db "/USR/INCLUDE/BOOT/CPU.H",0
EXTENDED_MSG: db "Extended half loaded", 0
INVALID_PATH: db "Fat32 read failed: ", 0


; Move all below into a new file

    ; call detect_vesa_mode
    ; call memmap
    ; call load_font
    ; call setup_paging
    ; call setup_interrupts

    ; mov ecx, 0xc0000080                 ; Select the Extended Feature Enable Register(EFER) Model Specific Register(MSR)
    ; rdmsr                               ; Read the register to ECX
    ; or eax, 0x00000100                  ; Flip the Long Mode Enable(LME) bit
    ; wrmsr                               ; Write changes back to EFER

    ; mov ebx, cr0                        ; Retrieve the contents of CR0 through EBX
    ; or ebx, 0x80000001                  ; Enable Paging and Protected mode simultaneously
    ; mov cr0, ebx                        ; Write changes back to CR0

    ; lgdt [GDT_DESCRIPTOR]


    ; jmp CODE_SEG:long_mode

; %include "./Real/memmap.asm"
; %include "./Real/load_font.asm"
; %include "./Real/vesa.asm"
; %include "./Real/paging.asm"
; %include "./Real/interrupts.asm"
; %include "./Real/gdt.asm"

; [bits 64]
; %include "./Real/sse.asm"
    
; SECTION .text
; ; [extern legacy_loader]
; long_mode:
;     mov ax, DATA_SEG
;     mov ds, ax
;     mov es, ax
;     mov fs, ax
;     mov gs, ax
;     mov ss, ax

;     mov rbp, 0x1E8480                   ; Set Stack at the top of the 2MB of Identity Mapped Ram
;     mov rsp, rbp                        ; Set Stack pointer at the Stack Base 
;     xor rbp, rbp                        ; Nullify the pointer to mark the beginning of the stack

;     call setup_sse
        
;     mov edi, 0xB8000                    ; Blank out the screen to a blue color.
;     mov rcx, 500                        ; Since we are clearing uint64_t over here, we put the count as Count/4.
;     mov rax, 0x1F201F201F201F20         ; Set the value to set the screen to: Blue background, white foreground, blank spaces.
;     rep stosq                           ; Clear the entire screen. 

;     jmp $

    ; jmp legacy_loader


; SECTION .data
; times 2048-($-$$) db 0                                  ; Pad for a minimum of 4 sectors for the extended half