
[extern _idt]
idt_descriptor:
    dw 4095               ; Maximum size of a 64 bit IDT
    dq _idt               ; Our idt pointer


GLOBAL load_IDT
load_IDT:
    lidt [idt_descriptor] ; Load the idt using the descriptor
    ret

%include "Interrupts/isr_init.asm"
%include "Interrupts/irq_init.asm"