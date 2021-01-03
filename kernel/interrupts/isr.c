#include <kernel/interrupts/idt.h>
#include <kernel/interrupts/isr.h>

#include <kernel/tty.h>
#include <kernel/vga.h>
#include <kernel/debug.h>

#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

const char *exception_messages[] = {
    "Division by Zero Exception",
    "Debug Exception",
    "Non Maskable Interrupt Exception",
    "Breakpoint Exception",
    "Into Detected Overflow Exception",
    "Out of Bounds Exception",
    "Invalid Opcode Exception",
    "No Coprocessor Exception",

    "Double Fault Exception",
    "Coprocessor Segment Overrun Exception",
    "Bad TSS Exception",
    "Segment Not Present Exception",
    "Stack Fault Exception",
    "General Protection Fault Exception",
    "Page Fault Exception",
    "Unknown Interrupt Exception",

    "Coprocessor Check Exception",
    "Machine Check Exception",
    "Reservered Exception",
    "Reservered Exception",
    "Reservered Exception",
    "Reservered Exception",
    "Reservered Exception",
    "Reservered Exception",

    "Reservered Exception",
    "Reservered Exception",
    "Reservered Exception",
    "Reservered Exception",
    "Reservered Exception",
    "Reservered Exception",
    "Reservered Exception",
};

void ISRs_install()
{
    printf("Installing ISR Gates\n");
    IDT_set_gate(0, (uint64_t)isr0, 0x08, 0x8E);
    IDT_set_gate(1, (uint64_t)isr1, 0x08, 0x8E);
    IDT_set_gate(2, (uint64_t)isr2, 0x08, 0x8E);
    IDT_set_gate(3, (uint64_t)isr3, 0x08, 0x8E);
    IDT_set_gate(4, (uint64_t)isr4, 0x08, 0x8E);
    IDT_set_gate(5, (uint64_t)isr5, 0x08, 0x8E);
    IDT_set_gate(6, (uint64_t)isr6, 0x08, 0x8E);
    IDT_set_gate(7, (uint64_t)isr7, 0x08, 0x8E);
    IDT_set_gate(8, (uint64_t)isr8, 0x08, 0x8E);
    IDT_set_gate(9, (uint64_t)isr9, 0x08, 0x8E);
    IDT_set_gate(10, (uint64_t)isr10, 0x08, 0x8E);
    IDT_set_gate(11, (uint64_t)isr11, 0x08, 0x8E);
    IDT_set_gate(12, (uint64_t)isr12, 0x08, 0x8E);
    IDT_set_gate(13, (uint64_t)isr13, 0x08, 0x8E); //general protection fault, triggers with irq1
    IDT_set_gate(14, (uint64_t)isr14, 0x08, 0x8E);
    IDT_set_gate(15, (uint64_t)isr15, 0x08, 0x8E);
    IDT_set_gate(16, (uint64_t)isr16, 0x08, 0x8E);
    IDT_set_gate(17, (uint64_t)isr17, 0x08, 0x8E);
    IDT_set_gate(18, (uint64_t)isr18, 0x08, 0x8E);
    IDT_set_gate(19, (uint64_t)isr19, 0x08, 0x8E);
    IDT_set_gate(20, (uint64_t)isr20, 0x08, 0x8E);
    IDT_set_gate(21, (uint64_t)isr21, 0x08, 0x8E);
    IDT_set_gate(22, (uint64_t)isr22, 0x08, 0x8E);
    IDT_set_gate(23, (uint64_t)isr23, 0x08, 0x8E);
    IDT_set_gate(24, (uint64_t)isr24, 0x08, 0x8E);
    IDT_set_gate(25, (uint64_t)isr25, 0x08, 0x8E);
    IDT_set_gate(26, (uint64_t)isr26, 0x08, 0x8E);
    IDT_set_gate(27, (uint64_t)isr27, 0x08, 0x8E);
    IDT_set_gate(28, (uint64_t)isr28, 0x08, 0x8E);
    IDT_set_gate(29, (uint64_t)isr29, 0x08, 0x8E);
    IDT_set_gate(30, (uint64_t)isr30, 0x08, 0x8E);
    IDT_set_gate(31, (uint64_t)isr31, 0x08, 0x8E);
}

static void page_fault(struct registers *regs)
{

    switch (regs->err_code)
    {
    case 0x0:
        printf("[Error Code: %x] Supervisory process tried to read a non-present page entry.\n", regs->err_code);
        break;
    case 0x1:
        printf("[Error Code: %x] Supervisory process tried to read a page and caused a protection fault.\n", regs->err_code);
        break;
    case 0x2:
        printf("[Error Code: %x] Supervisory process tried to write to a non-present page entry.\n", regs->err_code);
        break;
    case 0x3:
        printf("[Error Code: %x] Supervisory process tried to write a page and caused a protection fault.\n", regs->err_code);
        break;
    case 0x4:
        printf("[Error Code: %x] User process tried to read a non-present page entry.\n", regs->err_code);
        break;
    case 0x5:
        printf("[Error Code: %x] User process tried to read a page and caused a protection fault.\n", regs->err_code);
        break;
    case 0x6:
        printf("[Error Code: %x] User process tried to write to a non-present page entry.\n", regs->err_code);
        break;
    case 0x7:
        printf("[Error Code: %x] User process tried to write a page and caused a protection fault.\n", regs->err_code);
        break;
    default:
        printf("Unknown Page fault error. Error Code: %x\n", regs->err_code);
        break;
    }
    printf("[Instruction Pointer: 0x%x]\n\n", regs->rip);
}

static void general_protection_fault(struct registers *regs)
{
    printf("[Error code: 0x%x]\n", regs->err_code);
    printf("[Instruction Pointer: 0x%x]\n\n", regs->rip);
}

void fault_handler(struct registers *regs)
{

    if (regs->int_num < 32)
    {
        TTY_set_theme(VGA_COLOR_RED, VGA_COLOR_WHITE);
        printf("\nKernel Panic: %s\n\n System will now Halt\n\nDetails:\n\n", exception_messages[regs->int_num]);

        switch (regs->int_num)
        {
        case 13:
            general_protection_fault(regs);
            break;
        case 14:
            page_fault(regs);
            break;
        default:
            printf("[Error code: %x]\n", regs->err_code);
            break;
        }

        strace(10);

        TTY_set_theme(VGA_COLOR_BLACK, VGA_COLOR_GREEN);

        //If in graphics mode
        GRAPHICS_CONTEXT *ctx = get_graphics_ctx(SINGLE, 0, 0, get_screen_width(), get_screen_height());

        set_fill(ctx, 0x00f0f000);
        set_stroke(ctx, 0x0);

        int width = 400;
        int height = 200;
        int x = get_screen_width()/2 - width/2;
        int y = get_screen_height()/2 - height/2;
        fill_rect(ctx, x,y,width,height);
        set_origin(ctx, x,y);
        
        draw_text(ctx, CHAR_WIDTH, 0, "Kernel Panic: ");
        draw_text(ctx, CHAR_WIDTH, CHAR_HEIGHT, (char*)exception_messages[regs->int_num]);
        draw_text(ctx, CHAR_WIDTH, CHAR_HEIGHT * 2, "System will now halt");
        swap_buffer(ctx);
        destroy_graphics_ctx(ctx);

        while (1)
            ;
    }
}