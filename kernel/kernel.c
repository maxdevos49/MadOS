#include <kernel/tty.h>
#include <kernel/vga.h>

#include <kernel/debug.h>

#include <kernel/memory/memory.h>
#include <kernel/memory/heap.h>

#include <kernel/interrupts/idt.h>
#include <kernel/interrupts/isr.h>
#include <kernel/interrupts/irq.h>

#include <kernel/devices/keyboard.h>

#include <kernel/timer.h>
#include <kernel/time/rtc.h>
#include <kernel/time/pit.h>
#include <kernel/io.h>
#include <kernel/pci/core.h>
#include <kernel/filesystems/fs.h>
#include <kernel/filesystems/initrd.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// #ifdef __test
// //TODO relocate somewhere else
// #include <ctest.h>
// #include <string.h>

// void test_suites(void)
// {
//     printf("Testing <stdio.h>\n");
//     ctest_run_suite("int printf(const char*, ...)", test_printf);

//     printf("\nTesting <string.h>\n");
//     ctest_run_suite("size_t strlen(const char*)", test_strlen);
//     ctest_run_suite("int strcmp(const char*, const char *)", test_strcmp);
//     ctest_run_suite("void *memset(void*, int, size_t)", test_memset);
//     ctest_run_suite("void *memmove(void*, const void*, size_t))", test_memmove);

//     printf("\nTesting <stdlib.h>\n");
//     ctest_run_suite("int rand(void)", test_rand);
//     ctest_run_suite("int abs(int n)", test_abs);
//     ctest_run_suite("void* malloc(size_t)", test_malloc);
//     ctest_run_suite("void* calloc(size_t, size_t)", test_calloc);
//     ctest_run_suite("void* realloc(void*, size_t)", test_realloc);
//     ctest_run_suite("void free(void*)", test_free);

//     printf("\nTesting: <time.h>\n");
//     printf("\tTODO\n");
// }
// #endif

void irq8_handler()
{
    struct RTC_time time = RTC_read_time();
    printf("%d:%d:%d - %d/%d/%d\n", time.hour, time.minute, time.second, time.month, time.day, time.year);

    RTC_acknowledge_irq8();
}

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
    "|__/     |__/ \\_______/ \\_______/ \\______/  \\______/ \n";

void kernel_main(void)
{
    TTY_set_theme(VGA_COLOR_BLACK, VGA_COLOR_GREEN);
    TTY_init();

    printf("Booting Kernel\n");

    init_memory((struct mem_map_entry *)0x5000);

    struct mem_map_entry **usable_memory = get_usable_memory_regions();
    init_heap(usable_memory[1]->base_address, 0x10000);

    IDT_install();
    ISRs_install();
    IRQs_install();

    KB_install();
    TIMER_install();

    IRQ_enable();
    fs_root = INITRD_init();

    PCI_configure();
    
    printf("%s\n", splash);


    int i = 0;
    struct dirent *node = 0;
    while ((node = readdir_fs(fs_root, i)) != NULL)
    {
        printf("Found file: %s\n", node->name);

        struct fs_node *fsnode = finddir_fs(fs_root, node->name);
        // printf("node: %x\n", fsnode);
        if ((fsnode->flags & 0x7) == FS_DIRECTORY)
            printf("\t(directory)\n");
        else
        {
            printf("\t contents: \"");

            uint8_t buffer[256];
            // printf("Node loc: %x\n", fsnode);

            int sz = read_fs(fsnode, 0, 50, buffer);

            int j;
            for (j = 0; j < sz; j++)
                printf("%c", (buffer[j]));

            printf("\"\n");
        }

        i++;
    }
}