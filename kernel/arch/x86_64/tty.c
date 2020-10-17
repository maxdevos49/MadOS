#include <kernel/tty.h>
#include <stdio.h>
#include <string.h>

#include "vga.h"
#include "io.h"

static uint16_t cursor_position = 0;
static uint8_t current_theme = VGA_COLOR_BLUE | VGA_COLOR_WHITE << 4;

void terminal_initialize(void)
{
    terminal_set_cursor_position(0);
    terminal_clear();
}

void terminal_clear(void)
{
    uint64_t clear_color = current_theme;

    uint64_t value = 0;
    value += clear_color << 8;
    value += clear_color << 24;
    value += clear_color << 40;
    value += clear_color << 56;

    for (uint64_t *i = (uint64_t *)VGA_MEMORY; i < (uint64_t *)(VGA_MEMORY + 4000); i++)
    {
        *i = value;
    }
}

uint16_t terminal_get_cursor_position(void)
{
    return cursor_position;
}

void terminal_set_cursor_position(uint16_t position)
{
    outb(0x3d4, 0x0f); //TODO comeback and understand
    outb(0x3d5, (uint8_t)(position & 0xff));
    outb(0x3d4, 0x0e);
    outb(0x3d5, (uint8_t)((position >> 8) & 0xff));

    cursor_position = position;
}
/*
 * Special Characters:
 *      %%--> print a percent sign
 *      \a  --> audible alert
 *      \b  --> backspace
 *      \f  --> form feed
 *      \n  --> new line
 *      \r  --> carriage return
 *      \t  --> tab
 *      \v  --> vertical tab
 *      \\  --> backslash
 * */
void terminal_putchar(char c)
{
    uint16_t index = terminal_get_cursor_position();

    switch (c)
    {
    case '\n':
        index += VGA_WIDTH;

        if (index > (VGA_WIDTH * VGA_HEIGHT))
        {
            terminal_scroll();
            index = (VGA_WIDTH * VGA_HEIGHT);
        }

        break;
    case '\r':
        index -= index % VGA_WIDTH;
        break;
    case '\t':
        index += 4 - ((index & VGA_WIDTH) % 4);
        break;
    default:
        *(VGA_MEMORY + index * 2) = c;
        *(VGA_MEMORY + index * 2 + 1) = current_theme;
        index++;
        break;
    }

    terminal_set_cursor_position(index);
}

void terminal_write(const char *data, size_t size)
{
    uint64_t i;
    for (i = 0; i < size; i++)
        terminal_putchar(*(data + i));
}

void terminal_write_string(const char *str)
{
    terminal_write(str, strlen(str));
}

void terminal_scroll()
{
    uint16_t length = 3840;

    memmove(VGA_MEMORY, VGA_MEMORY + (VGA_WIDTH * 2), length);

    for (uint16_t i = 0; i < VGA_WIDTH; i++)
    {
        *(VGA_MEMORY + length + i * 2) = (char)' ';
    }
}

void terminal_set_theme(enum vga_color background, enum vga_color foreground)
{
    current_theme = foreground | background << 4;
}