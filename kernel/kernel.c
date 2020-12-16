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

    TTY_set_theme(VGA_COLOR_BLACK, VGA_COLOR_GREEN);
    TTY_init();

    printf("Booting Kernel\n");

    init_memory((struct mem_map_entry *)0x6000);

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

    VESA_install();

    // printf("%s\n", splash);

    uint16_t position = 0;
    uint16_t previous_position = 0;
    struct mouse_packet packet;
    struct vector2 mouse;
    struct vector2 scale;
    struct vector2 velocity;

    uint8_t previous_color = VGA_COLOR_GREEN | VGA_COLOR_BLACK << 4;

    uint8_t left_btn = 0;
    uint64_t click_count = 0;
    int16_t start_select;

    mouse.x = 0;
    mouse.y = 0;

    scale.x = 1080;
    scale.y = 720;

    while (true)
    {
        if (MOUSE_poll(&packet))
        {
            velocity.x = packet.x_movement;
            velocity.y = packet.y_movement;

            if (packet.x_sign == 1)
                velocity.x = velocity.x | 0xFFFFFFFFFFFFFF00;

            if (packet.y_sign == 1)
                velocity.y = velocity.y | 0xFFFFFFFFFFFFFF00;

            mouse.x += velocity.x;
            mouse.y -= velocity.y;

            if (mouse.x > scale.x)
                mouse.x = scale.x;

            if (mouse.x < 0)
                mouse.x = 0;

            if (mouse.y > scale.y)
                mouse.y = scale.y;

            if (mouse.y < 0)
                mouse.y = 0;

            struct vector2 mouse_s;
            mouse_s.x = (((VGA_WIDTH - 1) * mouse.x) / scale.x);
            mouse_s.y = (((VGA_HEIGHT - 1) * mouse.y) / scale.y);
            position = (mouse_s.y * VGA_WIDTH) + mouse_s.x;

            //Handle click
            if (packet.left_button == 1 && left_btn == 0)
            { //Mouse Down
                left_btn = 1;
                click_count = 1;

                int16_t start;
                int16_t end;

                if (start_select > previous_position)
                {
                    start = (start_select - abs(start_select - previous_position)) + 1;
                    end = start_select;
                }
                else
                {
                    start = start_select;
                    end = previous_position;
                }

                //Unselect
                for (int i = start; i < end; i++)
                    *(VGA_MEMORY + (i * 2) + 1) = VGA_COLOR_GREEN | VGA_COLOR_BLACK << 4;

                start_select = position;
                TTY_set_cursor_position((VGA_WIDTH * mouse_s.y) + mouse_s.x);
            }
            else if (packet.left_button == 1 && left_btn == 1)
            { //Mouse Drag
                click_count++;

                if (click_count > 1)
                {
                    TTY_set_cursor_position((VGA_WIDTH * mouse_s.y) + mouse_s.x);

                    int16_t start;
                    int16_t end;

                    if (start_select > previous_position)
                    {
                        start = (start_select - abs(start_select - previous_position)) + 1;
                        end = start_select;
                    }
                    else
                    {
                        start = start_select;
                        end = previous_position;
                    }

                    //Unselect
                    for (int i = start; i < end; i++)
                        *(VGA_MEMORY + (i * 2) + 1) = VGA_COLOR_GREEN | VGA_COLOR_BLACK << 4;

                    if (start_select > position)
                    {
                        start = start_select - abs(start_select - position) + 1;
                        end = start_select;
                    }
                    else
                    {
                        start = start_select;
                        end = position;
                    }

                    //Select
                    for (int i = start; i < end; i++)
                        *(VGA_MEMORY + (i * 2) + 1) = VGA_COLOR_WHITE | VGA_COLOR_BLUE << 4;
                }
            }
            else if (packet.left_button == 0 && left_btn == 1)
            { //Mouse Up
                left_btn = 0;
                click_count = 0;
            }

            //draw mouse
            *(VGA_MEMORY + (previous_position * 2) + 1) = previous_color;
            previous_color = *(VGA_MEMORY + (position * 2) + 1);
            *(VGA_MEMORY + (position * 2) + 1) = VGA_COLOR_BLACK | VGA_COLOR_WHITE << 4;
            previous_position = position;
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