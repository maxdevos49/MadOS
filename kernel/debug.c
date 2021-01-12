#include <kernel/debug.h>
#include <stdio.h>
#include <stdint.h>

void strace(uint32_t max_frames)
{

    struct stack_frame *stk;
    asm volatile("mov %%rbp,%0"
                 : "=r"(stk)::);

    printf("Stack trace: \n\t0x%08x \t %s - %s:%d\n", strace, __func__, __FILE__, 5);

    for (uint32_t frame = 0; (stk != NULL) && (frame < max_frames); ++frame)
    {
        //Unwind to previous stack frame
        printf("\t0x%08x \t %s - %s:%d\n", stk->eip, "??", "??", 0);
        stk = stk->ebp;
    }
}