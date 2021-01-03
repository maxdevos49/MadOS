#ifndef _STRING_H
#define _STRING_H 1

#include <sys/cdefs.h>

#include <stddef.h>
#include <stdint.h>

#define NULL 0

int memcmp(const void *, const void *, size_t);
void *memcpy(void *__restrict, const void *__restrict, size_t);
void *memmove(void *, const void *, size_t);
void *memset(void *, int, size_t);

void *memset64(void *, uint64_t, size_t); //Non standard

size_t strlen(const char *);
int strcmp(const char *s1, const char *s2);
char *strcpy(char *__restrict dst, const char *__restrict src);

#endif
