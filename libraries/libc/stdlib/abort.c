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
    // TTY_clear
    // TTY_set_cursor_position
    TTY_set_theme(VGA_COLOR_RED, VGA_COLOR_WHITE);
    printf("Kernel Panic: %s\n\n System will now Halt\n\n", msg);
    strace(10);
    TTY_set_theme(VGA_COLOR_BLACK, VGA_COLOR_GREEN);

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
