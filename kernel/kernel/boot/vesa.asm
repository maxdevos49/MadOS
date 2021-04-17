[bits 16]

VBE_INFO:
    .signature:		dd 0        ; indicate support for VBE 2.0+
    .version:       dw 0
    .oem:           dd 0
    .capabilities:  dd 0
    .video_modes:   dd 0
    .video_memory:  dw 0
    .software_rev:  dw 0
    .vendor:        dd 0
    .product_name:  dd 0
    .product_rev:   dd 0
    .table_data:	times 478 db 0
    GLOBAL VBE_INFO


VBE_MODE_INFO:
    .attributes:        dw 0
    .window_a:          db 0
    .window_b:          db 0
    .granularity:       dw 0
    .window_size:       dw 0
    .segment_a:         dw 0
    .segment_b:         dw 0
    .win_func_ptr:      dd 0
    .pitch:             dw 0
    .width:             dw 0; 15
    .height:            dw 0
    .w_char:            db 0
    .y_char:            db 0
    .planes:            db 0
    .bpp:               db 0;25
    .banks:             db 0
    .memory_model:      db 0
    .bank_size:         db 0
    .image_pages:       db 0
    .reserved0:         db 0
    .red_mask:          db 0
    .red_position:      db 0
    .green_mask:        db 0
    .green_position:    db 0
    .blue_mask:         db 0
    .blue_position:     db 0
    .reserved_mask:     db 0
    .reserved_position: db 0
    .direct_color_attributes:   db 0
    .framebuffer:               dd 0
    .off_screen_mem_off:        dd 0
    .off_screen_mem_size:       dw 0
    .reserved1:                 times 206 db 0
    GLOBAL VBE_MODE_INFO

FONT:
    times 4096 db 0
    GLOBAL FONT

    ; https://stanislavs.org/helppc/int_10-11.html
load_font:
    ; ask BIOS to return VGA bitmap fonts
    push ds
    push es
    ; ask BIOS to return VGA bitmap fonts
    mov ax, 1130h
    mov bh, 6
    int 10h
    ; copy charmap
    push es
    pop ds
    pop es
    mov si, bp
    mov cx, 256*16/4
    rep movsd
    pop	ds
    ret

detect_vesa_mode:
    mov [.width], ax    ; Store arguments
    mov [.height], bx
    mov [.bpp], cl

    sti                 ; Enable interupts

    push es                  ; some VESA BIOSes destroy ES, or so I read
    mov ax, 0x4F00           ; get VBE BIOS info
    mov di, VBE_INFO    
    int 0x10
    pop es

    cmp ax, 0x4F             ; BIOS doesn't support VBE?
    jne vesa_error

    mov ax, word[VBE_INFO.video_modes]
    mov [.offset], ax
    
    mov ax, word[VBE_INFO.video_modes+2]
    mov [.segment], ax

    mov ax, [.segment]
    mov fs, ax
    mov si, [.offset]

.find_mode:
    mov dx, [fs:si]
    add si, 2
    mov [.offset], si
    mov [.mode], dx
    mov ax, 0
    mov fs, ax

    cmp word [.mode], 0xFFFF ; end of list?
    je vesa_mode_not_found

    push es
    mov ax, 0x4F01           ; get VBE mode info
    mov cx, [.mode]
    mov di, VBE_MODE_INFO
    int 0x10
    pop es

    cmp ax, 0x4F
    jne vesa_error

    mov ax, [.width]
    cmp ax, [VBE_MODE_INFO.width]
    jne .next_mode

    mov ax, [.height]
    cmp ax, [VBE_MODE_INFO.height]
    jne .next_mode

    mov al, [.bpp]
    cmp al, [VBE_MODE_INFO.bpp]
    jne .next_mode

    ; Set the mode
    push es
    mov ax, 0x4F02
    mov bx, [.mode]
    or bx, 0x4000            ; enable LFB
    mov di, 0                ; not sure if some BIOSes need this... anyway it doesn't hurt
    int 0x10
    pop es

    cmp ax, 0x4F
    jne vesa_error

    clc                      ; Clear carry

    ret

.next_mode:
    mov ax, [.segment]
    mov fs, ax
    mov si, [.offset]
    jmp .find_mode


.width				dw 0
.height				dw 0
.bpp				db 0
.segment			dw 0
.offset				dw 0
.mode				dw 0

vesa_error:
    mov bx, VESA_ERROR_MSG
    call print_string
    jmp $

vesa_mode_not_found:
    mov bx, VESA_NOT_FOUND_MSG
    call print_string
    jmp $

    ; Global variables
VESA_MSG: db "Loading VESA information",0xA, 0xD,0
VESA_ERROR_MSG: db "Failed to retrieve VESA BIOS information",0xA, 0xD,0
VESA_NOT_FOUND_MSG: db "Failed to find a desired VESA Mode",0xA, 0xD,0
TRY: db "TRY",0xA, 0xD,0