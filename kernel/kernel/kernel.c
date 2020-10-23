#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <kernel/tty.h>
#include <kernel/debug.h>
#include <kernel/memory.h>
#include <kernel/heap.h>

#if defined(__x86_64)
#include "../arch/x86_64/idt/idt.h"
// #include "../arch/x86_64/keyboard.h"
#endif

#ifdef __test
//TODO relocate somewhere else
#include <ctest.h>
#include <string.h>

void test_suites(void)
{
    printf("Testing <stdio.h>\n");
    ctest_run_suite("int printf(const char*, ...)", test_printf);

    printf("\nTesting <string.h>\n");
    ctest_run_suite("size_t strlen(const char*)", test_strlen);
    ctest_run_suite("int strcmp(const char*, const char *)", test_strcmp);
    ctest_run_suite("void *memset(void*, int, size_t)", test_memset);
    ctest_run_suite("void *memmove(void*, const void*, size_t))", test_memmove);

    printf("\nTesting <stdlib.h>\n");
    ctest_run_suite("int rand(void)", test_rand);
    ctest_run_suite("int abs(int n)", test_abs);
    ctest_run_suite("void* malloc(size_t)", test_malloc);
    ctest_run_suite("void* calloc(size_t, size_t)", test_calloc);
    ctest_run_suite("void* realloc(void*, size_t)", test_realloc);
    ctest_run_suite("void free(void*)", test_free);

    printf("\nTesting: <time.h>\n");
    printf("\tTODO\n");
}
#endif

char *splash = //Load from binary?
    "=============================================================================="
    "\nWelcome to\n"
    " /$$      /$$                 /$$  /$$$$$$   /$$$$$$ \n"
    "| $$$    /$$$                | $$ /$$__  $$ /$$__  $$\n"
    "| $$$$  /$$$$  /$$$$$$   /$$$$$$$| $$  \\ $$| $$  \\__/\n"
    "| $$ $$/$$ $$ |____  $$ /$$__  $$| $$  | $$|  $$$$$$ \n"
    "| $$  $$$| $$  /$$$$$$$| $$  | $$| $$  | $$ \\____  $$\n"
    "| $$\\  $ | $$ /$$__  $$| $$  | $$| $$  | $$ /$$  \\ $$\n"
    "| $$ \\/  | $$|  $$$$$$$|  $$$$$$$|  $$$$$$/|  $$$$$$/\n"
    "|__/     |__/ \\_______/ \\_______/ \\______/  \\______/ \n\n";

void kernel_main(void)
{
    terminal_set_theme(VGA_COLOR_BLACK, VGA_COLOR_GREEN);
    init_terminal();
    // init_memory((struct mem_map_entry*)0x5000);

    // print_all_memory_map((struct mem_map_entry*)0x5000);

    // struct mem_map_entry **usable_memory = get_usable_memory_regions();
    // uint64_t heap_base = usable_memory[1]->base_address;
    // uint64_t heap_size = usable_memory[1]->region_length;

    // init_heap(heap_base, heap_size);

#if defined(__x86_64)
    idt_install();
    isrs_install();
    irq_install();
    asm volatile("sti");
    printf("Test\n");

    // init_keyboard();
#endif

    printf("%d", 1 / 0);

    // printf("%s\n", splash);

#ifdef __test
    // run_ctest(test_suites);
#endif
}