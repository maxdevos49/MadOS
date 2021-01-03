#include <stdio.h>
#include <string.h>

int puts(const char *str)
{

	// int len = strlen(str);
	int i;
	for (i = 0; str[i] != '\0'; i++)
		putchar(str[i]);

	return i+1;
}
