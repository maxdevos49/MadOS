[bits 16]
; ==================    Load Font    =========================
; Loads a bitmap font for use in a later VESA Graphics mode
; Input: void
; Output: void
; ============================================================
SECTION .text
load_font:
    pusha

    xor ax, ax      ; Setup Segment
    mov es, ax      ; "              "
    mov di, FONT    ; Set Segment index (es:di == 0000:FONT)

    push ds
    push es

    mov ax, 1130h   ; Select BIOS VGA function
    mov bh, 6       ; Select Bitmap font
    int 10h         ; Execute!

    ; copy font bitmap to reserved section
    push es
    pop ds
    pop es
    
    mov si, bp
    mov cx, 256*16/4
    rep movsd
    pop	ds

    popa
    ret

SECTION .bss
GLOBAL FONT
FONT: resb 4096