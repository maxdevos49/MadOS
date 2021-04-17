#include <stdio.h>
#include <string.h>

#if defined(__is_libk)
#include <kernel/devices/tty.h>
#endif

int puts(const char *str)
{

	int len = strlen(str);
	// int i;
	// for (i = 0; str[i] != '\0'; i++)
	// 	putchar(str[i]);

#if defined(__is_libk)

	TTY_write(str,len);

#else
	// TODO: Implement stdio and the write system call.
#endif

	return len + 1;
}
