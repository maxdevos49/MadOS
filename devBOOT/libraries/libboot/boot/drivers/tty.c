#include "tty.h"
#include "../string.h"

static TTY_CONFIG *tty;

int TTY_init(TTY_CONFIG *config)
{
    if (config == NULL)
        return 1; // NULL

    if (config->char_width == 0 || config->char_height == 0)
        return 2;

    if (config->set_cursor == NULL || config->put_char == NULL || config->clear == NULL || config->write == NULL || config->scroll == NULL)
        return 3;

    tty = config;

    return 0;
}

void TTY_clear(void)
{
    tty->clear(tty);
}

uint16_t TTY_get_cursor_position(void)
{
    return tty->cursor_position;
}

void TTY_set_cursor_position(uint32_t position)
{
    if (position > tty->char_width * tty->char_height - 1)
    {
        TTY_scroll();
        position = tty->char_width * tty->char_height - tty->char_width;
    }

    tty->cursor_position = position;
    tty->set_cursor(tty);
}

void TTY_putchar(char c)
{
    uint16_t index = TTY_get_cursor_position();

    switch (c)
    {
    case '\n':

        index += tty->char_width;         //\n
        index -= index % tty->char_width; //implied \r
        TTY_set_cursor_position(index-1);

        break;
    case '\r':
        index -= index % tty->char_width;
        TTY_set_cursor_position(index);

        break;
    case '\t':
        index += 4 - ((index & tty->char_width) % 4);
        TTY_set_cursor_position(index);

        break;
    case 0x08: //Backspace
        index--;
        tty->put_char(tty, ' ', index);
        TTY_set_cursor_position(index);

        break;
    default:
        tty->put_char(tty, c, index);
        TTY_set_cursor_position(index+1);

        break;
    }
}

void TTY_write(const char *data, size_t size)
{
    tty->write(tty, data, size);
}

void TTY_write_string(const char *str)
{
    TTY_write(str, strlen(str));
}

void TTY_scroll()
{
    tty->scroll(tty);
}

uint32_t TTY_get_char_width()
{
    return tty->char_width;
}

uint32_t TTY_get_char_height()
{
    return tty->char_height;
}