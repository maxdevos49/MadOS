#ifndef _KERNEL_DEBUG_H
#define _KERNEL_DEBUG_H 1

#include <stdint.h>

//https://wiki.osdev.org/Stack

struct stack_frame
{
    struct stack_frame *ebp;
    uint32_t eip;
};

void strace(uint32_t max_frames);

#endif