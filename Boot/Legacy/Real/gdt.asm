; ; ==================    Start GDT    ====================
; GDT:
;     .NULL:
;         dq 0x0000000000000000
;     .CODE:
;         dq 0x00209A0000000000   ; 64-bit code descriptor (exec/read).
;     .DATA:
;         dq 0x0000920000000000   ; 64-bit data descriptor (read/write).
 
; ALIGN 4
;     dw 0                              ; Padding to make the "address of the GDT" field aligned on a 4-byte boundary
 
; .DESCRIPTOR:
;     dw $ - GDT - 1                    ; 16-bit Size (Limit) of GDT.
;     dd GDT                            ; 32-bit Base Address of GDT. (CPU will zero extend to 64-bit)

;     ; ==================     End GDT     ====================

