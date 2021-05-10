
void _start() {

    while (1)  asm ("hlt");
}


// #include <kernel/devices/tty.h>
// #include <kernel/devices/vga.h>

// #include <kernel/debug.h>

// #include <kernel/memory/memory.h>
// #include <kernel/memory/heap.h>

// #include <kernel/interrupts/idt.h>
// #include <kernel/interrupts/isr.h>
// #include <kernel/interrupts/irq.h>

// #include <kernel/devices/keyboard.h>

// #include <kernel/timer.h>
// #include <kernel/time/rtc.h>
// #include <kernel/time/pit.h>
// #include <kernel/io.h>
// #include <kernel/pci/core.h>
// #include <kernel/filesystems/fs.h>
// #include <kernel/filesystems/initrd.h>
// #include <kernel/devices/mouse.h>
// #include <kernel/devices/vesa.h>

// #include <stdint.h>
// #include <string.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>
// #include <graphics.h>

// #include "pacman/pacman.h"

// struct vector2
// {
//     int64_t x;
//     int64_t y;
// };

// char *splash =
//     "======================================================\n"
//     "Welcome to\n"
//     " /$$      /$$                 /$$  /$$$$$$   /$$$$$$ \n"
//     "| $$$    /$$$                | $$ /$$__  $$ /$$__  $$\n"
//     "| $$$$  /$$$$  /$$$$$$   /$$$$$$$| $$  \\ $$| $$  \\__/\n"
//     "| $$ $$/$$ $$ |____  $$ /$$__  $$| $$  | $$|  $$$$$$ \n"
//     "| $$  $$$| $$  /$$$$$$$| $$  | $$| $$  | $$ \\____  $$\n"
//     "| $$\\  $ | $$ /$$__  $$| $$  | $$| $$  | $$ /$$  \\ $$\n"
//     "| $$ \\/  | $$|  $$$$$$$|  $$$$$$$|  $$$$$$/|  $$$$$$/\n"
//     "|__/     |__/ \\_______/ \\_______/ \\______/  \\______/ \n>";

// void kernel_main(void)
// {
//     TTY_CONFIG tty_config;
//     VGA_tty_init(&tty_config);
//     TTY_init(&tty_config);

//     printf("Booting Kernel\n");

//     init_memory((struct mem_map_entry *)0x6000);

//     struct mem_map_entry **usable_memory = get_usable_memory_regions();
//     init_heap(usable_memory[1]->base_address, usable_memory[1]->region_length);

//     VESA_install();
//     GRAPHICS_CONTEXT *tty_ctx = get_graphics_ctx(DOUBLE, get_screen_width() / 2, 0, get_screen_width() / 2, get_screen_height());
//     VESA_tty_init(&tty_config, tty_ctx);
//     TTY_init(&tty_config);

//     IDT_install();
//     ISRs_install();
//     IRQs_install();
//     TIMER_install();
//     KB_install();
//     MOUSE_install();

//     IRQ_enable();
//     // fs_root = INITRD_init();
//     // PCI_configure();

//     printf("%s", splash);


//     // GRAPHICS_CONTEXT *ctx = get_graphics_ctx(DOUBLE, get_screen_width() / 2, get_screen_height() / 2, get_screen_width() / 2, get_screen_height() / 2);

//     // set_fill(ctx, 0x00ff0000);
//     // fill_rect(ctx, 0, 0, get_ctx_width(ctx), get_ctx_width(ctx));

//     // draw_text(ctx, 0, 0, "MadOS Kernel");
//     // draw_text(ctx, 0, CHAR_HEIGHT, "Context 2: Rectangle Demo");

//     // for (int i = 0; i < 100; i++)
//     // {
//     //     int color = rand() % 0xffffffff;
//     //     int x = rand() % get_ctx_width(ctx);
//     //     int y = rand() % get_ctx_height(ctx) + CHAR_HEIGHT * 2;
//     //     int size = rand() % 50;
//     //     set_fill(ctx, color);
//     //     fill_rect(ctx, x, y, size, size);
//     // }

//     // swap_buffer(ctx);
//     // destroy_graphics_ctx(ctx); //Cleans up context

//     pacman_main(0, NULL); //Aka context 3

//     while(1);
//     __builtin_unreachable();

//     abort("Kernel Exited Unexpectedly");
//     // uint16_t position = 0;
//     // uint16_t previous_position = 0;
//     // struct mouse_packet packet;
//     // struct vector2 mouse;
//     // struct vector2 scale;
//     // struct vector2 velocity;

//     // uint8_t previous_color = VGA_COLOR_GREEN | VGA_COLOR_BLACK << 4;

//     // uint8_t left_btn = 0;
//     // uint64_t click_count = 0;
//     // int16_t start_select;

//     // mouse.x = 0;
//     // mouse.y = 0;

//     // scale.x = 1080;
//     // scale.y = 720;

//     // while (true)
//     // {
//     //     if (MOUSE_poll(&packet))
//     //     {
//     //         velocity.x = packet.x_movement;
//     //         velocity.y = packet.y_movement;

//     //         if (packet.x_sign == 1)
//     //             velocity.x = velocity.x | 0xFFFFFFFFFFFFFF00;

//     //         if (packet.y_sign == 1)
//     //             velocity.y = velocity.y | 0xFFFFFFFFFFFFFF00;

//     //         mouse.x += velocity.x;
//     //         mouse.y -= velocity.y;

//     //         if (mouse.x > scale.x)
//     //             mouse.x = scale.x;

//     //         if (mouse.x < 0)
//     //             mouse.x = 0;

//     //         if (mouse.y > scale.y)
//     //             mouse.y = scale.y;

//     //         if (mouse.y < 0)
//     //             mouse.y = 0;

//     //         clear_rect(ctx,0,0,get_ctx_width(ctx), get_ctx_height(ctx));
//     //         set_fill(ctx, 0x00ff00ff);
//     //         fill_rect(ctx, mouse.x, mouse.y, 10,10);
//     //         swap_buffer(ctx);

//     //     }
//     // }

//     // int i = 0;
//     // struct dirent *node = 0;
//     // while ((node = readdir_fs(fs_root, i)) != NULL)
//     // {
//     //     printf("Found file: %s\n", node->name);

//     //     struct fs_node *fsnode = finddir_fs(fs_root, node->name);
//     //     // printf("node: %x\n", fsnode);
//     //     if ((fsnode->flags & 0x7) == FS_DIRECTORY)
//     //         printf("\t(directory)\n");
//     //     else
//     //     {
//     //         printf("\t contents: \"");

//     //         uint8_t buffer[256];
//     //         // printf("Node loc: %x\n", fsnode);

//     //         int sz = read_fs(fsnode, 0, 50, buffer);

//     //         int j;
//     //         for (j = 0; j < sz; j++)
//     //             printf("%c", (buffer[j]));

//     //         printf("\"\n");
//     //     }

//     //     i++;
//     // }
// }