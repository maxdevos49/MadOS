#include <stdint.h>
#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void)
{
 
    terminal_initialize();
    putchar('M');


    // printf("Hello, kernel World!\n");
}