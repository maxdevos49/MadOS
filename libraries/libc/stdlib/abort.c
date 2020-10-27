#include <stdio.h>
#include <stdlib.h>

#if defined(__is_libk)
#include <kernel/tty.h>
#include <kernel/debug.h>
#endif

__attribute__((__noreturn__)) void abort(const char * msg)
{
#if defined(__is_libk)
    // TODO: Add proper kernel panic.
    // terminal_clear();
    // terminal_set_cursor_position(0);
    terminal_set_theme(VGA_COLOR_RED, VGA_COLOR_WHITE);
    printf("Kernel Panic: %s\n\n System will now Halt\n\n", msg);
    trace_stack_trace(10);
    terminal_set_theme(VGA_COLOR_BLACK, VGA_COLOR_GREEN);

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
