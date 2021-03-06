#ifndef __BOOT_STRING_H
#define __BOOT_STRING_H 1

#include "types.h"

int memcmp(const void *, const void *, size_t);
void *memcpy(void *__restrict, const void *__restrict, size_t);
void *memmove(void *, const void *, size_t);
void *memset(void *, int, size_t);

size_t strlen(const char *);
int strcmp(const char *s1, const char *s2);
char *strcpy(char *__restrict dst, const char *__restrict src);


#endif