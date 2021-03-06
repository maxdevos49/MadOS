#include "vga.h"
#include "tty.h"

static uint8_t theme;
static void set_cursor(TTY_CONFIG *config);
static void put_char(TTY_CONFIG *config, char c, uint32_t position);
static void clear(TTY_CONFIG *config);
static void write(TTY_CONFIG *config, const char *data, size_t size);
static void scroll(TTY_CONFIG *config);
static uint8_t color32_to_color8(uint32_t color);

void VGA_tty_init(TTY_CONFIG *config)
{
    config->char_width = VGA_WIDTH;
    config->char_height = VGA_HEIGHT;
    config->cursor_position = 0;

    config->set_cursor = set_cursor;
    config->put_char = put_char;
    config->clear = clear;
    config->write = write;
    config->scroll = scroll;

    theme = color32_to_color8(0x00ffffff) | color32_to_color8(0x00000000) << 4;

    return;
}

static void set_cursor(TTY_CONFIG *config)
{
    outb(0x3d4, 0x0f);
    outb(0x3d5, (uint8_t)(config->cursor_position & 0xff));
    outb(0x3d4, 0x0e);
    outb(0x3d5, (uint8_t)((config->cursor_position >> 8) & 0xff));
}

static void put_char(TTY_CONFIG *config, char c, uint32_t position)
{
    *(VGA_MEMORY + config->cursor_position * 2) = c;
    *(VGA_MEMORY + config->cursor_position * 2 + 1) = theme;
}

static void clear(TTY_CONFIG *config)
{
    uint64_t clear_color = theme;

    uint64_t value = 0;
    value += clear_color << 8;
    value += clear_color << 24; //24 because color data is every other byte
    value += clear_color << 40;
    value += clear_color << 56;

    int size = (config->char_width * 2) * config->char_height;

    for (uint64_t *i = (uint64_t *)VGA_MEMORY; i < (uint64_t *)(VGA_MEMORY + size); i++)
    {
        *i = value;
    }
}

static void write(TTY_CONFIG *config, const char *data, size_t size)
{
    uint64_t i;
    for (i = 0; i < size; i++)
        TTY_putchar(*(data + i));
}

static void scroll(TTY_CONFIG *config)
{
    uint64_t *src_ptr = (uint64_t *)VGA_MEMORY;
    uint64_t *dst_ptr = (uint64_t *)(VGA_MEMORY - (config->char_width * 2));
    uint16_t length = config->char_width * config->char_height * 2;

    memmove(dst_ptr, src_ptr, length);

    for (int i = 0; i < config->char_width; i++)
    {
        int offset = length - config->char_width * 2;

        *(VGA_MEMORY + offset + i * 2) = ' ';
        *(VGA_MEMORY + offset + i * 2 + 1) = theme;
    }
}

static uint8_t color32_to_color8(uint32_t color)
{
    uint8_t red = (color & 0x00ff0000) >> 16;
    uint8_t green = (color & 0x0000ff00) >> 8;
    uint8_t blue = (color & 0x000000ff);

    uint8_t index = (red > 128 | green > 128 | blue > 128) ? 8 : 0; // Bright bit

    index |= (red > 64) ? 4 : 0;   // Red bit
    index |= (green > 64) ? 2 : 0; // Green bit
    index |= (blue > 64) ? 1 : 0;  // Blue bit
    return index;
}