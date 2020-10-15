#include <kernel/debug.h>
#include <stdio.h>
#include <stdint.h>

void trace_stack_trace(uint32_t max_frames)
{

    struct stack_frame *stk;
    asm volatile("mov %%rbp,%0"
        : "=r"(stk)::);

    printf("Stack trace:\n");

    for (uint32_t frame = 0; stk && frame < max_frames; frame++)
    {
        //Unwind to previous stack frame
        printf("  %x\n", stk->eip);//Ignore vscode complaining because its assigned with the inline assembly
        stk = stk->ebp;
    }
}