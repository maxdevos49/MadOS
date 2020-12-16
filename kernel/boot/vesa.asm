[bits 16]

VBE_INFO:
    .signature:		resb 4 ; indicate support for VBE 2.0+
    .version:       resb 2
    .oem:           resb 4
    .capabilities:  resb 4
    .video_modes:   resb 4
    .video_memory:  resb 2
    .software_rev:  resb 2
    .vendor:        resb 4
    .product_name:  resb 4
    .product_rev:   resb 4
    .table_data:	resb 512-34
    GLOBAL VBE_INFO

VBE_MODE_INFO:
    .attributes:        resb 2
    .window_a:          resb 1
    .window_b:          resb 1
    .granularity:       resb 2
    .window_size:       resb 2
    .segment_a:         resb 2
    .segment_b:         resb 2
    .win_func_ptr:      resb 4
    .pitch:             resb 2
    .width:             resb 2
    .height:            resb 2
    .w_char:            resb 1
    .y_char:            resb 1
    .planes:            resb 1
    .bpp:               resb 1
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
    .framebuffer:               resb 4
    .off_screen_mem_off:        resb 4
    .off_screen_mem_size:       resb 2
    .reserved1:                 resb 206
    GLOBAL VBE_MODE_INFO

current_mode: resb 2

 detect_vesa_mode:
    mov bx, VESA_MSG
    call print_string

    ; Get Vesa info
    mov ax, 0
    mov es, ax
    mov di, VBE_INFO
    mov ax, 0x4f00
    int 0x10
    cmp ax, 0x4f
    jne vesa_error

    ; .find_mode:

    ; ;TODO

    ; mov ax,  word [current_mode]
    ; cmp ax, 0xffff
    ; jne .find_mode

    ; Set Mode
    mov ax, 0x4f02
    mov bx, 0x4198
    int 0x10
    cmp ax, 0x4f
    jne vesa_error

    ; Get Mode Info
    mov ax, 0
    mov es, ax
    mov di, VBE_MODE_INFO
    mov ax, 0x4f01
    mov cx, 0x0198
    int 0x10
    cmp ax, 0x4f
    jne vesa_error

    ret


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
