#include <stdio.h>

#if defined(__is_libk)
#include <kernel/tty.h>
#endif

int putchar(int c)
{
#if defined(__is_libk)

    if ((char)c == '\n')
        terminal_putchar('\r');

    terminal_putchar((char)c);

#else
    // TODO: Implement stdio and the write system call.
#endif

    return c;
}
