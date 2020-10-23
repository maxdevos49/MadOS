[bits 16]

mov bx, EXTENDED_SPACE_SUCCESS_MSG
call print_string

jmp enter_protected_mode

enter_protected_mode:
    call detect_memory                    ; Detects memory used
    call enable_A20                       ; Allows use of extended memory

    cli                                   ; We must switch off interrupts until we have set-up the protected mode
                                          ; otherwise interrupts will run riot.
    mov bx, LOADING_GDT_MSG
    call print_string

    lgdt [gdt_descriptor]                 ; Load our global descriptor table, which defines the
                                          ; protected mode segments (e.g. for code and data)
    mov eax, cr0                          ; To make the switch to protected mode, we set
    or eax, 0x1                           ; the first bit of CRO, a control register
    mov cr0, eax

    jmp CODE_SEG:init_32_bit              ; Make a far jump(i.e. to a new segment) to out 32-bit
                                          ; code. This also forces the CPU to flush its cache of
                                          ; pre-fetched and real-mode decoded instructions, which
                                          ; can cause problems
enable_A20:

    mov bx, ENABLE_A20_MSG
    call print_string

    in al, 0x92                           ; Allows for extended memory access on some machines
    or al, 2
    out 0x92, al
    ret

%include "../bootloader/print.asm"
%include "./gdt.asm"
%include "./detect_memory.asm"

; ; 16 BIT Global Variables
EXTENDED_SPACE_SUCCESS_MSG: db "We are successfully in extended space",0xA, 0xD, 0
ENABLE_A20_MSG: db "Enabling", 132, " A20",0xA, 0xD, 0
LOADING_GDT_MSG: db "Loading GDT",0xA, 0xD, 0


[bits 32]

init_32_bit:

    mov ax, DATA_SEG                      ; Now in PM our old segments are meaningless,
    mov ds, ax                            ; so we point our segment registers to the
    mov ss, ax                            ; data sector we defined in our GDT
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000                      ; Update our stack position so it is right at the top of free space
    mov esp, ebp

    call detect_cpu_id
    call detect_long_mode
    call setup_identity_paging
    call edit_gdt

    jmp CODE_SEG:init_64_bit


%include "./cpu_id.asm"
%include "./paging.asm"

[bits 64]
[extern kernel_main]

%include "../idt/idt_init.asm"

init_64_bit:
    call activate_SSE
    xor ebp, ebp                          ; Mark it as null
    call kernel_main
    jmp $

activate_SSE:                             ; Allows for use of the math co-processor?
    mov rax, cr0
    and ax, 0b11111101
    or ax, 0b00000001
    mov cr0, rax

    mov rax, cr4
    or ax, 0b1100000000
    mov cr4, rax

    ret

