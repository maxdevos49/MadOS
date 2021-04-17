    ; In 64 bit protected mode we do not have the instructions 
    ; pusha or popa so we must define macros
%macro PUSHALL 0
    push rax
    push rbx
    push rcx
    push rdx
    push rbp
    
    push rsi
    push rdi

    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro

%macro POPALL 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8

    pop rdi
    pop rsi

    pop rbp
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro