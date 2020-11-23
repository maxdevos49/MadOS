#ifndef _STRING_H
#define _STRING_H 1

#include <sys/cdefs.h>

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    int memcmp(const void *, const void *, size_t);
    void *memcpy(void *__restrict, const void *__restrict, size_t);
    void *memmove(void *, const void *, size_t);
    void *memset(void *, int, size_t);
    void *memset64(void *, uint64_t, size_t);
    size_t strlen(const char *);
    int strcmp(const char *, const char *);
    char *strcpy(char *__restrict, const char *__restrict);

#ifndef NULL
#define NULL 0
#endif

#ifdef __test
    void test_strcmp(void);
    void test_strlen(void);
    void test_memset(void);
    void test_memmove(void);
#endif

#ifdef __cplusplus
}
#endif

#endif
