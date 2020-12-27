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

    typedef struct _io_buffer
    {
        //TODO
    } FILE;

    int printf(const char *__restrict, ...);
    int putchar(int);
    int puts(const char *);

#if defined(__cplusplus)
}
#endif

#endif