#include <stdint.h>

// #include <kernel/tty.h>
#define VGA_MEMORY (uint8_t *)0xb8000

extern void kernel_main(void)
{
    uint16_t index = 0;
    while (1)
    {
        *(VGA_MEMORY + index * 2) = 'H';
        index++;
    }


    // terminal_initialize();
    // printf("Hello, kernel World!\n");
}