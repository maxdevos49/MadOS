#include <stdint.h>

// #include <kernel/tty.h>
#define VGA_MEMORY (uint8_t *)0xb8000

void kernel_main(void)
{
    uint16_t index = 0;
    while (1)
    {
        *(VGA_MEMORY + index * 2 + 1) = 0x10 | 0x00;
        index++;
    }

    // terminal_initialize();
    // printf("Hello, kernel World!\n");
}