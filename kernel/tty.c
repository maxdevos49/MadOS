#include <kernel/tty.h>
#include <stdio.h>
#include <string.h>

#include <kernel/vga.h>
#include <kernel/io.h>

static uint16_t cursor_position = 0;
static uint8_t current_theme = VGA_COLOR_GREEN | VGA_COLOR_BLACK << 4;

void TTY_init(void)
{
    TTY_set_cursor_position(0);
    TTY_clear();
}

void TTY_clear(void)
{
    uint64_t clear_color = current_theme;

    uint64_t value = 0;
    value += clear_color << 8;
    value += clear_color << 24; //24 because color data is every other byte
    value += clear_color << 40;
    value += clear_color << 56;

    for (uint64_t *i = (uint64_t *)VGA_MEMORY; i < (uint64_t *)(VGA_MEMORY + 4000); i++)
    {
        *i = value;
    }
}

uint16_t TTY_get_cursor_position(void)
{
    return cursor_position;
}

void TTY_set_cursor_position(uint16_t position)
{

    if (position > VGA_WIDTH * VGA_HEIGHT - 1)
    {
        TTY_scroll();
        position = VGA_WIDTH * VGA_HEIGHT - VGA_WIDTH;
    }

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
 * */
void TTY_putchar(char c)
{
    uint16_t index = TTY_get_cursor_position();

    switch (c)
    {
    case '\n':
        index += VGA_WIDTH;
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

    TTY_set_cursor_position(index);
}

void TTY_write(const char *data, size_t size)
{
    uint64_t i;
    for (i = 0; i < size; i++)
        TTY_putchar(*(data + i));
}

void TTY_write_string(const char *str)
{
    TTY_write(str, strlen(str));
}

void TTY_scroll()
{
    uint64_t *src_ptr = (uint64_t *)VGA_MEMORY;
    uint64_t *dst_ptr = (uint64_t *)(VGA_MEMORY - (VGA_WIDTH * 2));
    uint16_t length = VGA_WIDTH * VGA_HEIGHT * 2;

    memmove(dst_ptr, src_ptr, length);
    // memset64(VGA_MEMORY + length - (VGA_WIDTH * 2), 0x00, (VGA_WIDTH * 2));
    for (int i = 0; i < VGA_WIDTH; i++)
    {
        int offset = length - VGA_WIDTH*2;

        // printf("Offset: %d", offset);
        *(VGA_MEMORY + offset + i * 2) = ' ';
        *(VGA_MEMORY + offset + i * 2 + 1) = current_theme;
    }
}

void TTY_set_theme(enum vga_color background, enum vga_color foreground)
{
    current_theme = foreground | background << 4;
}