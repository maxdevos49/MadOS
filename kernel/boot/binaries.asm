[bits 64]

%macro INC_BIN 2
    SECTION .rodata
    GLOBAL %1
%1:
    incbin %2
    db 0
    %1_size: dq %1_size - %1
%endmacro

; INC_BIN initrd_data_header, "boot/initrd.img"
; INC_BIN map, "boot/map.tga"