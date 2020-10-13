#include <stdint.h>
#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void)
{
    terminal_initialize();
    printf("printf('%c', \"%s\", %f, %d, %x);", 'A', "String", 45.45f, 1234, 0xface);


}