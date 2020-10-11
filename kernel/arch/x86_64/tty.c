#include <kernel/tty.h>
#include <stdint.h>
#include "vga.h"
#include "io.h"

static uint16_t cursor_position = 0;
static uint8_t current_theme = 0x10 | 0x0f;

void terminal_initialize(void)
{
    terminal_set_cursor_position(0);
    terminal_set_theme(VGA_COLOR_BLUE, VGA_COLOR_WHITE);
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

void terminal_putchar(char c)
{
    uint16_t index = terminal_get_cursor_position();

    switch (c)
    {

    case 10: //\n
        index += VGA_WIDTH;
        break;
    case 13: //\r
        index -= index % VGA_WIDTH;
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
    uint8_t *char_ptr = (uint8_t *)str;
    while (*char_ptr != 0)
    {
        terminal_putchar(*char_ptr);
        char_ptr++;
    }
    //TODO change to
    //terminal_write(str, strlen(str));
}

void terminal_scroll()
{
    //TODO requires memcopy or something else
}

void terminal_set_theme(enum vga_color background, enum vga_color foreground)
{
    current_theme = foreground | background << 4;
}