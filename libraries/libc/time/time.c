#include <time.h>
#include <stdlib.h>

#if defined(__is_libk)
#include <kernel/timer.h>
#endif

time_t time(time_t *args)
{
    time_t t;

#if defined(__is_libk)
    t = TIMER_get_time();
#else
    // TODO: Implement syscall.
    abort("Syscall Not Implemented.");
#endif

    if (args != NULL)
        *args = t;

    return t;
}