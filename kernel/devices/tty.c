#include <kernel/devices/tty.h>

#include <string.h>

static TTY_CONFIG *tty;

void TTY_init(TTY_CONFIG *config)
{
    tty = config;

    TTY_set_theme(0, 0x00ffffff);
    TTY_clear();
    TTY_set_cursor_position(0);
    TTY_set_theme(0, 0x00ffffff);
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
    start_batch(tty->tty_ctx, 56734);

    if (position > tty->char_width * tty->char_height - 1)
    {
        TTY_scroll();
        position = tty->char_width * tty->char_height - tty->char_width;
    }

    tty->cursor_position = position;
    tty->set_cursor(tty);

    end_batch(tty->tty_ctx, 56734);
}

void TTY_putchar(char c)
{
    uint16_t index = TTY_get_cursor_position();

    switch (c)
    {
    case '\n':

        index += tty->char_width;         //\n
        index -= index % tty->char_width; //implied \r
        TTY_set_cursor_position(index);

        break;
    case '\r':
        index -= index % tty->char_width;
        TTY_set_cursor_position(index);

        break;
    case '\t':
        index += 4 - ((index & tty->char_width) % 4);
        TTY_set_cursor_position(index);

        break;
    case 0x08://Backspace
        index--;
        tty->put_char(tty, ' ', index);
        TTY_set_cursor_position(index);

        break;
    default:
        index++;
        uint32_t position = index - 1;
        TTY_set_cursor_position(index);

        tty->put_char(tty, c, position);
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

void TTY_set_theme(uint32_t background, uint32_t foreground)
{
    if (tty->apply_theme != NULL)
        tty->apply_theme(background, foreground);

    tty->foreground_theme = foreground;
    tty->background_theme = background;
    tty->apply_theme(background, foreground);
}

uint32_t TTY_get_char_width()
{
    return tty->char_width;
}

uint32_t TTY_get_char_height()
{
    return tty->char_height;
}