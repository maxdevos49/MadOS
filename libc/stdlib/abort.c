#include <stdio.h>
#include <stdlib.h>

#if defined(__is_libk)
#include <kernel/tty.h>
#include <kernel/debug.h>
#endif

__attribute__((__noreturn__)) void abort(void)
{
#if defined(__is_libk)
    // TODO: Add proper kernel panic.
    terminal_clear();
    terminal_set_cursor_position(0);
    printf("kernel: panic: abort()\n");
    trace_stack_trace(10);
#else
    // TODO: Abnormally terminate the process as if by SIGABRT.
    printf("\nabort()\n");
#endif
    while (1)
    {
#if defined(__is_libk)
    
#endif
    }
    __builtin_unreachable();
}
