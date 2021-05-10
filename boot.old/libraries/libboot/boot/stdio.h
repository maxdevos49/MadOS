#ifndef __BOOT_STDIO_H
#define __BOOT_STDIO_H 1

#include "stdarg.h"

#include "stdio/_file.h"

typedef struct __file_stream FILE;

int sprintf(char *buffer, const char *format, ...);
int vsprintf(char *buffer, const char *format, va_list args);

int printf(const char *restrict format, ...);
int serial_printf(const char *restrict format, ...);

#endif