#include <stdio.h>
#include <stdlib.h>

#if defined(__is_libk)
#include <kernel/devices/tty.h>
#include <kernel/debug.h>
#endif

__attribute__((__noreturn__)) void abort(const char *msg)
{
#if defined(__is_libk)
    TTY_set_theme(0x00ff0000, 0xffffffff);
    printf("Kernel Panic: %s\n\n System will now Halt\n\n", msg);
    strace(10);
#else
    // TODO: Abnormally terminate the process as if by SIGABRT.
    printf("\nabort()\n");
#endif

    while (1)
        ;
        
    __builtin_unreachable();
}
