#include <kernel/devices/keyboard.h>
#include <kernel/devices/KB1.h>
#include <kernel/io.h>
#include <kernel/interrupts/irq.h>
#include <kernel/devices/tty.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <kernel/debug.h>
#include <kernel/timer.h>

static void (*active_handler)(uint8_t scan_code, char chr);

bool caps_lock = false;
bool left_shift_pressed = false;
bool right_shift_pressed = false;
bool left_alt_pressed = false;
bool right_alt_pressed = false;

uint8_t last_scan_code;

void standard_keyboard_handler(uint8_t scan_code, char chr)
{

    if (chr != 0)
    {
        putchar(chr);
    }
    else
    {
        switch (scan_code)
        {
        case 0x2a: //left shift press
            left_shift_pressed = true;
            break;
        case 0xaa: //left shift release
            left_shift_pressed = false;
            break;
        case 0x36: //right shift press
            right_shift_pressed = true;
            break;
        case 0xb6: //right shift release
            right_shift_pressed = false;
            break;
        }
    }
}

void keyboard_handler_0xE0(uint8_t scan_code)
{
    switch (scan_code)
    {
    case 0x50: //down arrow
        TTY_set_cursor_position(TTY_get_cursor_position() + TTY_get_char_width());
        break;
    case 0x4B: //left arrow
        TTY_set_cursor_position(TTY_get_cursor_position() - 1);
        break;
    case 0x4D: //right arrow
        TTY_set_cursor_position(TTY_get_cursor_position() + 1);
        break;
    case 0x48: //up arrow
        TTY_set_cursor_position(TTY_get_cursor_position() - TTY_get_char_width());
        break;
    }
}

void keyboard_handler()
{

    uint8_t scan_code = inb(0x60);
    uint8_t chr = 0;

    if (left_shift_pressed || right_shift_pressed)
        chr = KB1_table.shift_map[scan_code];
    else if (left_alt_pressed || right_alt_pressed)
        chr = KB1_table.alt_map[scan_code];
    else
        chr = KB1_table.map[scan_code];

    if (active_handler == NULL)
    {
        switch (last_scan_code)
        {
        case 0xe0:
            keyboard_handler_0xE0(scan_code);
            break;
        default:
            standard_keyboard_handler(scan_code, chr);
            break;
        }
    }
    else
    {
        active_handler(scan_code, chr);
    }

    last_scan_code = scan_code;
}

void register_kb_handler(void (*handler)(uint8_t scan_code, char chr))
{
    active_handler = handler;
}

void KB_install(void)
{
    active_handler = NULL;
    printf("Installing Keyboard\n");
    IRQ_install_handler(1, keyboard_handler);
}