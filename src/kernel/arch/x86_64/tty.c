#include <kernel/tty.h>
#include <stdint.h>

static uint16_t cursor_position;

void terminal_initialize(void)
{
    cursor_position = 0;

    //TODO implement
}

uint16_t terminal_cursor_position(void)
{
    return cursor_position;
}

void terminal_clear(void)
{

}

void terminal_set_cursor_position(uint16_t position)
{
    
}

void terminal_set_cursor_cords(uint8_t x, uint8_t y)
{

}

void terminal_putchar(char c)
{
    //TODO implement
}

void terminal_write(const char *data, size_t size)
{
    //TODO implement
}

void terminal_writestring(const char *data)
{
    //TODO implement
}