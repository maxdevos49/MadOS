#include <stdio.h>

#if defined(__is_libk)
#include <kernel/tty.h>
#endif

int putchar(int c)
{
#if defined(__is_libk)

    if ((char)c == '\n')
        TTY_putchar('\r');

    TTY_putchar(c);

#else
    // TODO: Implement stdio and the write system call.
#endif

    return c;
}
