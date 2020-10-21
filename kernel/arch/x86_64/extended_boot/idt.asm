
[extern _idt]
idt_descriptor:
    dw 4095         ; Maximum size of a 64 bit IDT
    dq _idt         ; Our idt pointer??

    ; In 64 bit protected mode we do not have the instructions 
    ; pusha or popa so we must define macros
    %macro PUSHALL 0
        push rax
        push rcx
        push rdx
        push r8
        push r9
        push r10
        push r11
    %endmacro

    %macro POPALL 0
        pop r11
        pop r10
        pop r9
        pop r8
        pop rdx
        pop rcx
        pop rax

    %endmacro

[extern isr1_handler]

isr1:                    ; Interupt service routine 1
    PUSHALL              ; Save registers on stack
    call isr1_handler    ; Call our handler for the interupt routine
    POPALL               ; Restore registers from stack
    iretq                ; 64 bit Interupt return
    GLOBAL isr1

load_IDT:
    lidt [idt_descriptor] ; Load the idt using the descriptor
    sti                  ; Enable interupts
    ret
    GLOBAL load_IDT