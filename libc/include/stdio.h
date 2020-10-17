#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>
#include <stdint.h>
#include <stddef.h>

#define EOF (-1)

#if defined(__cplusplus)
extern "C"
{
#endif

    int printf(const char *__restrict, ...);
    int putchar(int);
    int puts(const char *);

#ifdef __test
    void test_printf();
// char* test_putchar();
// char* test_puts();
#endif

#if defined(__cplusplus)
}
#endif

#endif