#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    int abs(int n);

    int rand(void);
    void srand(unsigned int seed);

    void *malloc(size_t size);
    void *realloc(void *ptr, size_t size);
    void *calloc(size_t num, size_t size);
    void free(void *ptr);

    __attribute__((__noreturn__)) void abort(const char*);

#define RAND_MAX 2147483647
#ifndef NULL
#define NULL 0
#endif

#if __test
    void test_rand(void);
    void test_abs(void);
    void test_malloc(void);
    void test_calloc(void);
    void test_realloc(void);
    void test_free(void);
#endif

#ifdef __cplusplus
}
#endif

#endif
