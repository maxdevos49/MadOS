#include <stdio.h>
#include <string.h>

int puts(const char *str)
{

	int len = strlen(str);
	int i;
	for (i = 0; i < len; i++)
		putchar(str[i]);

	return len;
}
