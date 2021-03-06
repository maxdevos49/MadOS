#ifndef __BOOT_DRIVERS_TTY
#define __BOOT_DRIVERS_TTY 1

#include "../types.h"

typedef struct __tty_config TTY_CONFIG;

struct __tty_config
{
    uint32_t char_width;
    uint32_t char_height;
    uint32_t cursor_position;

    void (*set_cursor)(TTY_CONFIG *config);
    void (*put_char)(TTY_CONFIG *config, char c, uint32_t position);
    void (*clear)(TTY_CONFIG *config);
    void (*write)(TTY_CONFIG *config, const char *data, size_t size);
    void (*scroll)(TTY_CONFIG *config);
    void (*apply_theme)(uint32_t background, uint32_t foreground);
};

void TTY_init(TTY_CONFIG *config);

uint16_t TTY_get_cursor_position();
void TTY_set_cursor_position(uint32_t position);
void TTY_putchar(char c);
void TTY_clear();
void TTY_write(const char *data, size_t size);
void TTY_write_string(const char *data);
void TTY_scroll();
uint32_t TTY_get_char_width();
uint32_t TTY_get_char_height();

#endif