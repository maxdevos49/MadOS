[bits 16]
; ==================   Detect Vesa Mode   ====================
; Loads the desired VESA Mode from the given contstants. Halts if VESA 
; is not suported or the desired VESA mode is not found
; Input: void
; Output: void
; ============================================================
; CONSTANTS
WIDTH equ 1024;px
HEIGHT equ 768;px
BPP equ 32
; ============================================================

SECTION .text
detect_vesa_mode:
    mov word [FRAMEBUFFER_INFO.width], WIDTH    ; Store arguments
    mov word [FRAMEBUFFER_INFO.height], HEIGHT
    mov byte [FRAMEBUFFER_INFO.bpp], BPP

    sti                 ; Enable interupts

    push es                  ; some VESA BIOSes destroy ES, or so I read
    mov ax, 0x4F00           ; get VBE BIOS info
    mov di, VBE    
    int 0x10
    pop es

    cmp ax, 0x4F             ; BIOS doesn't support VBE?
    jne .vesa_error

    mov ax, word[VBE.video_modes]
    mov [FRAMEBUFFER_INFO.offset], ax
    
    mov ax, word[VBE.video_modes+2]
    mov [FRAMEBUFFER_INFO.segment], ax

    mov ax, [FRAMEBUFFER_INFO.segment]
    mov fs, ax
    mov si, [FRAMEBUFFER_INFO.offset]

.find_mode:
    mov dx, [fs:si]
    add si, 2
    mov [FRAMEBUFFER_INFO.offset], si
    mov [FRAMEBUFFER_INFO.mode], dx
    mov ax, 0
    mov fs, ax

    cmp word [FRAMEBUFFER_INFO.mode], 0xFFFF ; end of list?
    je .vesa_mode_not_found

    push es
    mov ax, 0x4F01           ; get VBE mode info
    mov cx, [FRAMEBUFFER_INFO.mode]
    mov di, VBE_MODE_INFO
    int 0x10
    pop es

    cmp ax, 0x4F
    jne .vesa_error

    mov ax, [FRAMEBUFFER_INFO.width]
    cmp ax, [VBE_MODE_INFO.width]
    jne .next_mode

    mov ax, [FRAMEBUFFER_INFO.height]
    cmp ax, [VBE_MODE_INFO.height]
    jne .next_mode

    mov al, [FRAMEBUFFER_INFO.bpp]
    cmp al, [VBE_MODE_INFO.bpp]
    jne .next_mode

    mov ax, [VBE_MODE_INFO.pitch]
    mov word [FRAMEBUFFER_INFO.pitch], ax

    mov eax, [VBE_MODE_INFO.framebuffer]
    mov DWORD [FRAMEBUFFER_INFO.buffer], eax

    ; Set the mode
    ; push es
    ; mov ax, 0x4F02
    ; mov bx, [FRAMEBUFFER_INFO.mode]
    ; or bx, 0x4000            ; enable LFB
    ; mov di, 0                ; not sure if some BIOSes need this... anyway it doesn't hurt
    ; int 0x10
    ; pop es

    ; cmp ax, 0x4F
    ; jne .vesa_error

    ; clc                      ; Clear carry

    ret

.next_mode:
    mov ax, [FRAMEBUFFER_INFO.segment]
    mov fs, ax
    mov si, [FRAMEBUFFER_INFO.offset]
    jmp .find_mode

.vesa_error:
    mov bx, .error_MSG
    call print
    jmp $

.vesa_mode_not_found:
    mov bx, .VESA_NOT_FOUND_MSG
    call print
    jmp $

SECTION .rodata
    .error_MSG: db "[ERROR] Vesa mode failed to be set", 0xA, 0xD, 0
    .VESA_NOT_FOUND_MSG: db "[ERROR] Failed to find a desired VESA Mode", 0xA, 0xD, 0

SECTION .bss
GLOBAL FRAMEBUFFER_INFO
FRAMEBUFFER_INFO:
    .width:         resw 1
    .height:        resw 1
    .bpp:           resb 1
    .mode:          resw 1
    .pitch:         resw 1
    .buffer:        resd 1
    .segment:       resw 1
    .offset:        resw 1

VBE:
    .signature:		resd 1        ; indicate support for VBE 2.0+
    .version:       resw 1
    .oem:           resd 1
    .capabilities:  resd 1
    .video_modes:   resd 1
    .video_memory:  resw 1
    .software_rev:  resw 1
    .vendor:        resd 1
    .product_name:  resd 1
    .product_rev:   resd 1
    .table_data:	resb 478

VBE_MODE_INFO:
    .attributes:        resw 1
    .window_a:          resb 1
    .window_b:          resb 1
    .granularity:       resw 1
    .window_size:       resw 1
    .segment_a:         resw 1
    .segment_b:         resw 1
    .win_func_ptr:      resd 1
    .pitch:             resw 1
    .width:             resw 1; 15
    .height:            resw 1
    .w_char:            resb 1
    .y_char:            resb 1
    .planes:            resb 1
    .bpp:               resb 1;25
    .banks:             resb 1
    .memory_model:      resb 1
    .bank_size:         resb 1
    .image_pages:       resb 1
    .reserved0:         resb 1
    .red_mask:          resb 1
    .red_position:      resb 1
    .green_mask:        resb 1
    .green_position:    resb 1
    .blue_mask:         resb 1
    .blue_position:     resb 1
    .reserved_mask:     resb 1
    .reserved_position: resb 1
    .direct_color_attributes:   resb 1
    .framebuffer:               resd 1
    .off_screen_mem_off:        resd 1
    .off_screen_mem_size:       resw 1
    .reserved1:                 resb 206