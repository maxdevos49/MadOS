#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H 1

#include <stddef.h>
#include <stdint.h>

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

void TTY_init();

uint16_t TTY_get_cursor_position();
void TTY_set_cursor_position(uint16_t position);
void TTY_putchar(char c);
void TTY_clear();
void TTY_write(const char *data, size_t size);
void TTY_write_string(const char *data);
void TTY_scroll();
void TTY_set_theme(enum vga_color background, enum vga_color foreground);

#endif
