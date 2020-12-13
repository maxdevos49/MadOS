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
#include <kernel/devices/mouse.h>

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

struct vector2
{
    int64_t x;
    int64_t y;
};

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

    TIMER_install();

    IRQ_enable();
    fs_root = INITRD_init();

    PCI_configure();
    KB_install();
    MOUSE_install();

    printf("%s\n", splash);

    struct mouse_packet packet;
    struct vector2 mouse;
    mouse.x = 0;
    mouse.y = 0;
    uint16_t previous_position = 0;
    while (true)
    {
        if (MOUSE_poll(&packet))
        {
            int16_t x_mov = packet.x_movement;
            int16_t y_mov = packet.y_movement;

            if (packet.x_sign == 1)
                x_mov = x_mov | 0xFFFFFF00;

            if (packet.y_sign == 1)
                y_mov = y_mov | 0xFFFFFF00;

            mouse.x += x_mov;
            mouse.y -= y_mov;

            int64_t scale = 1000;

            if (mouse.x > scale)
                mouse.x = scale;

            if (mouse.x < 0)
                mouse.x = 0;

            if (mouse.y > scale)
                mouse.y = scale;

            if (mouse.y < 0)
                mouse.y = 0;

            uint16_t cursor_p = TTY_get_cursor_position();

            TTY_set_cursor_position(120);
            printf("X: %d", mouse.x);
            TTY_set_cursor_position(200);
            printf("Y: %d", mouse.y);

            struct vector2 mouse_s;
            mouse_s.x = (((VGA_WIDTH - 1) * mouse.x) / scale);
            mouse_s.y = (((VGA_HEIGHT - 1) * mouse.y) / scale);

            TTY_set_cursor_position(140);
            printf("X: %d", mouse_s.x);
            TTY_set_cursor_position(220);
            printf("Y: %d", mouse_s.y);
            TTY_set_cursor_position(cursor_p);

            //draw mouse
            *(VGA_MEMORY + (previous_position * 2) + 1) = VGA_COLOR_GREEN | VGA_COLOR_BLACK << 4;
            int16_t position = (mouse_s.y * VGA_WIDTH) + mouse_s.x;
            previous_position = position;
            *(VGA_MEMORY + (position * 2) + 1) = VGA_COLOR_BLACK | VGA_COLOR_WHITE << 4;
        }
    }

    // int i = 0;
    // struct dirent *node = 0;
    // while ((node = readdir_fs(fs_root, i)) != NULL)
    // {
    //     printf("Found file: %s\n", node->name);

    //     struct fs_node *fsnode = finddir_fs(fs_root, node->name);
    //     // printf("node: %x\n", fsnode);
    //     if ((fsnode->flags & 0x7) == FS_DIRECTORY)
    //         printf("\t(directory)\n");
    //     else
    //     {
    //         printf("\t contents: \"");

    //         uint8_t buffer[256];
    //         // printf("Node loc: %x\n", fsnode);

    //         int sz = read_fs(fsnode, 0, 50, buffer);

    //         int j;
    //         for (j = 0; j < sz; j++)
    //             printf("%c", (buffer[j]));

    //         printf("\"\n");
    //     }

    //     i++;
    // }
}