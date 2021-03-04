[bits 16]
; =====================   Extended Half   =======================
; Begins with the assumption that long mode is supported 
; for the current machine. Sets up gdt, paging, and long
; mode for handing off to the kernel loader.
; 
; Input: void
; Output: void
;
; ===============================================================
SECTION .text
jmp extended                            ; Extended boot entry

%include "./Real/print.asm"
%include "./Real/memmap.asm"
%include "./Real/load_font.asm"
%include "./Real/vesa.asm"
%include "./Real/paging.asm"
%include "./Real/interrupts.asm"
%include "./Real/gdt.asm"

SECTION .text
extended:
    mov bx, extended_MSG
    call print

    ; call detect_vesa_mode
    call memmap
    call load_font
    call setup_paging
    call setup_interrupts

    mov ecx, 0xc0000080                 ; Select the Extended Feature Enable Register(EFER) Model Specific Register(MSR)
    rdmsr                               ; Read the register to ECX
    or eax, 0x00000100                  ; Flip the Long Mode Enable(LME) bit
    wrmsr                               ; Write changes back to EFER

    mov ebx, cr0                        ; Retrieve the contents of CR0 through EBX
    or ebx, 0x80000001                  ; Enable Paging and Protected mode simultaneously
    mov cr0, ebx                        ; Write changes back to CR0

    lgdt [GDT.DESCRIPTOR]

    jmp CODE_SEG:long_mode

SECTION .rodata
extended_MSG: db "Extended half loaded", 0xA, 0xD, 0

[bits 64]
%include "./Real/sse.asm"
    
SECTION .text
[extern bios_loader]
long_mode:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov rbp, 0x1E8480                   ; Set Stack at the top of the 2MB of Identity Mapped Ram
    mov rsp, rbp                        ; Set Stack pointer at the Stack Base 
    xor rbp, rbp                        ; Nullify the pointer to mark the beginning of the stack

    call setup_sse
        
    mov edi, 0xB8000                    ; Blank out the screen to a blue color.
    mov rcx, 500                        ; Since we are clearing uint64_t over here, we put the count as Count/4.
    mov rax, 0x1F201F201F201F20         ; Set the value to set the screen to: Blue background, white foreground, blank spaces.
    rep stosq                           ; Clear the entire screen. 


    jmp bios_loader


SECTION .data
times 2048-($-$$) db 0                                  ; Pad for a minimum of 4 sectors for the extended half