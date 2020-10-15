#ifndef _KERNEL_DEBUG_H
#define _KERNEL_DEBUG_H

#include <stdint.h>

//https://wiki.osdev.org/Stack

struct stack_frame
{
    struct stack_frame *ebp;
    uint32_t eip;
};

void trace_stack_trace(uint32_t);

#endif