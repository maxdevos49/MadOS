#include <kernel/debug.h>
#include <stdio.h>
#include <stdint.h>

void strace(uint32_t max_frames)
{

    struct stack_frame *stk;
    asm volatile("mov %%rbp,%0"
                 : "=r"(stk)::);

    printf("Stack trace: %x \n", strace);

    for (uint32_t frame = 0; (stk != NULL) && (frame < max_frames); ++frame)
    {
        // printf("%x --- \t",stk);

        //Unwind to previous stack frame
        printf("  0x%x\n", stk->eip); //Ignore vscode complaining because its assigned with the inline assembly
        stk = stk->ebp;
    }
}