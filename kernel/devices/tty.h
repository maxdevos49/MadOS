#ifndef _KERNEL_DEVICES_TTY_H
#define _KERNEL_DEVICES_TTY_H 1

#include <stddef.h>
#include <stdint.h>

#include <graphics.h>

typedef struct __tty_config TTY_CONFIG;
typedef struct __tty_config
{
	uint32_t char_width;
	uint32_t char_height;
	uint32_t cursor_position;
	uint32_t foreground_theme;
	uint32_t background_theme;
    GRAPHICS_CONTEXT* tty_ctx;


	void (*set_cursor)(TTY_CONFIG* config);
	void (*put_char)(TTY_CONFIG* config, char c, uint32_t position);
	void (*clear)(TTY_CONFIG* config);
	void (*write)(TTY_CONFIG* config, const char *data, size_t size);
	void (*scroll)(TTY_CONFIG* config);
	void (*apply_theme)( uint32_t background, uint32_t foreground);
} TTY_CONFIG;

void TTY_init(TTY_CONFIG *config);
// void TTY_uninit();

uint16_t TTY_get_cursor_position();
void TTY_set_cursor_position(uint32_t position);
void TTY_putchar(char c);
void TTY_clear();
void TTY_write(const char *data, size_t size);
void TTY_write_string(const char *data);
void TTY_scroll();
void TTY_set_theme(uint32_t background, uint32_t foreground);
uint32_t TTY_get_char_width();
uint32_t TTY_get_char_height();


#endif
