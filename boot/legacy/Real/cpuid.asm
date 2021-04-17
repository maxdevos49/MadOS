[bits 16]
; ==================     Check CPUID    =======================
; Checks the cpu for feature support of cpuid itself and
; long mode. Halts if either feature is not present.
; Input: void
; Output: void
; ===========================================================
SECTION .text
check_cpuid:
    
    pushfd                      ; Get flags in EAX register.
    pop eax                     ; "            "

    mov ecx, eax                ; Move flags to ECX for comparing later

    xor eax, 1 << 21            ; Flip the ID bit
    push eax                    ; Restore flags from stack
    popfd                       ; "             "
 
    pushfd                      ; Get flags into EAX again
    pop eax                     ; "             "

    push ecx                    ; Restore flags to there original state
    popfd                       ; "              "

    xor ecx, eax                ; if (ecx == eax) then
    jz .no_cpuid                ; cpuid is not supported

    mov eax, 0x80000000         ; Select the cpuid "extended functions support" function
    cpuid                       ; Execute cpuid function 0x80000000
    cmp eax, 0x80000001         ; If ((unsigned)eax < 0x80000001) then
    jb .no_longmode             ; long mode is not supported

    mov eax, 0x80000001         ; Select the cpuid "longmode support" function
    cpuid                       ; Execute cpuid function 0x80000001
    test edx, 1 << 29           ; if(edx[bit 29] == 0) then
    jz .no_longmode             ; long mode is not supported

    ret
 
   .no_cpuid:
        mov bx, .no_cpuid_MSG
        call print
        jmp $

    .no_longmode:
        mov bx, .no_longmode_MSG
        call print
        jmp $

    .no_cpuid_MSG: db "[ERROR] CPUID not supported", 0
    .no_longmode_MSG: db "[ERROR] Long mode not supported", 0
