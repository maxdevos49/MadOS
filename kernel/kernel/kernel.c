#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <kernel/tty.h>
#include <kernel/debug.h>

#if defined(__x86_64)
#include "../arch/x86_64/IDT.h"
#include "../arch/x86_64/keyboard.h"
#endif

void kernel_main(void)
{
    terminal_initialize();
    
#if defined(__x86_64)
    init_IDT();
    init_keyboard();
#endif

    abort();
}