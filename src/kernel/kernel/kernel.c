#include <stdint.h>

#include <kernel/tty.h>

void kernel_main(void)
{
 
    terminal_initialize();
    terminal_write_string("Hello, World!\r\n");

    terminal_set_theme(VGA_COLOR_RED, VGA_COLOR_LIGHT_BLUE);
    
    terminal_write_string("Hello, World again!\r\n");


    // printf("Hello, kernel World!\n");
}