#include "keyboard.h"
#include "kb_1.h"
#include "idt/idt.h"
#include "io.h"

#include <kernel/tty.h>
#include "vga.h"

#include <stdio.h>
#include <stdbool.h>

bool left_shift_pressed = false;
bool right_shift_pressed = false;
uint8_t last_scan_code;

void standard_keyboard_handler(uint8_t scan_code, char chr)
{

    if (chr != 0)
    {

        switch (left_shift_pressed || right_shift_pressed)
        {
        case true:
            putchar(chr - 32);
            break;
        case false:
            putchar(chr);
            break;
        }
    }
    else
    {
        switch (scan_code)
        {
        case 0x8e: //Backspace press
            terminal_set_cursor_position(terminal_get_cursor_position() - 1);
            putchar(' ');
            terminal_set_cursor_position(terminal_get_cursor_position() - 1);
            break;
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
        case 0x9c: //enter
            putchar('\n');
            break;
        }
    }
}

void keyboard_handler_0xE0(uint8_t scan_code)
{
    switch (scan_code)
    {
    case 0x50: //down arrow
        terminal_set_cursor_position(terminal_get_cursor_position() + VGA_WIDTH);
        break;
    case 0x4B: //left arrow
        terminal_set_cursor_position(terminal_get_cursor_position() - 1);
        break;
    case 0x4D: //right arrow
        terminal_set_cursor_position(terminal_get_cursor_position() + 1);
        break;
    case 0x48: //up arrow
        terminal_set_cursor_position(terminal_get_cursor_position() - VGA_WIDTH);
        break;
    }
}

void keyboard_handler()
{

    uint8_t scan_code = inb(0x60);
    uint8_t chr = 0;

    if (scan_code < 0x3a)
        chr = kb1_table[scan_code];

    switch (last_scan_code)
    {
    case 0xe0:
        keyboard_handler_0xE0(scan_code);
        break;
    default:
        standard_keyboard_handler(scan_code, chr);
        break;
    }

    last_scan_code = scan_code;
}

void install_keyboard()
{
    printf("Installing Keyboard\n");
    irq_install_handler(1, keyboard_handler);
}