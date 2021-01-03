#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>
#include <stddef.h>

#define RAND_MAX 2147483647
#define NULL 0

int abs(int n);

int rand(void);
void srand(unsigned int seed);

void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void *calloc(size_t num, size_t size);
void free(void *ptr);

__attribute__((__noreturn__)) void abort(const char *);

#endif
