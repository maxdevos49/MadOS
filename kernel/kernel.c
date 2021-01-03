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
#include <kernel/devices/vesa.h>

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <graphics.h>

#include "pacman/pacman.h"

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

char *splash =
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
    TTY_init();
    printf("Booting Kernel\n");

    init_memory((struct mem_map_entry *)0x6000);

    struct mem_map_entry **usable_memory = get_usable_memory_regions();
    init_heap(usable_memory[1]->base_address, usable_memory[1]->region_length);

    IDT_install();
    ISRs_install();
    IRQs_install();

    TIMER_install();
    KB_install();
    MOUSE_install();

    IRQ_enable();
    // fs_root = INITRD_init();
    // PCI_configure();

    VESA_install();

    GRAPHICS_CONTEXT *ctx = get_graphics_ctx(DOUBLE, get_screen_width() / 2, 0, get_screen_width() / 2, get_screen_height() / 2);

    char buffer[1000];

    sprintf(buffer, "Width: 0x%06x, Height: %8d, Test Float: %2.9f\n", get_screen_width(), get_screen_height(), 22241.22987f);
    printf(buffer);


    draw_text(ctx, 0, 0, buffer);

    draw_text(ctx, 0, CHAR_HEIGHT, "Context 1: Line Demo");

    move_to(ctx, get_ctx_width(ctx) / 2, get_ctx_height(ctx) / 2);
    set_line_width(ctx, 2);
    for (int i = 0; i < 25; i++)
    {
        int x = (rand() % get_ctx_width(ctx) - 20) + 10;
        int y = (rand() % get_ctx_height(ctx) - 20) + 10;

        line_to(ctx, x, y);
    }

    swap_buffer(ctx);

    destroy_graphics_ctx(ctx); //Cleans up context

    ctx = get_graphics_ctx(DOUBLE, get_screen_width() / 2, get_screen_height() / 2, get_screen_width() / 2, get_screen_height() / 2);

    set_fill(ctx, 0x00ff0000);
    fill_rect(ctx, 0, 0, get_ctx_width(ctx), get_ctx_width(ctx));

    draw_text(ctx, 0, 0, "MadOS Kernel");
    draw_text(ctx, 0, CHAR_HEIGHT, "Context 2: Rectangle Demo");

    for (int i = 0; i < 100; i++)
    {
        int color = rand() % 0xffffffff;
        int x = rand() % get_ctx_width(ctx);
        int y = rand() % get_ctx_height(ctx) + CHAR_HEIGHT * 2;
        int size = rand() % 50;
        set_fill(ctx, color);
        fill_rect(ctx, x, y, size, size);
    }

    swap_buffer(ctx);
    destroy_graphics_ctx(ctx); //Cleans up context

    pacman_main(0, NULL); //Aka context 3

    // uint16_t position = 0;
    // uint16_t previous_position = 0;
    // struct mouse_packet packet;
    // struct vector2 mouse;
    // struct vector2 scale;
    // struct vector2 velocity;

    // uint8_t previous_color = VGA_COLOR_GREEN | VGA_COLOR_BLACK << 4;

    // uint8_t left_btn = 0;
    // uint64_t click_count = 0;
    // int16_t start_select;

    // mouse.x = 0;
    // mouse.y = 0;

    // scale.x = 1080;
    // scale.y = 720;

    // while (true)
    // {
    //     if (MOUSE_poll(&packet))
    //     {
    //         velocity.x = packet.x_movement;
    //         velocity.y = packet.y_movement;

    //         if (packet.x_sign == 1)
    //             velocity.x = velocity.x | 0xFFFFFFFFFFFFFF00;

    //         if (packet.y_sign == 1)
    //             velocity.y = velocity.y | 0xFFFFFFFFFFFFFF00;

    //         mouse.x += velocity.x;
    //         mouse.y -= velocity.y;

    //         if (mouse.x > scale.x)
    //             mouse.x = scale.x;

    //         if (mouse.x < 0)
    //             mouse.x = 0;

    //         if (mouse.y > scale.y)
    //             mouse.y = scale.y;

    //         if (mouse.y < 0)
    //             mouse.y = 0;

    //         clear_rect(ctx,0,0,get_ctx_width(ctx), get_ctx_height(ctx));
    //         set_fill(ctx, 0x00ff00ff);
    //         fill_rect(ctx, mouse.x, mouse.y, 10,10);
    //         swap_buffer(ctx);

    //     }
    // }

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