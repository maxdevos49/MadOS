#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <kernel/tty.h>
#include <kernel/debug.h>

#if defined(__x86_64)
#include "../arch/x86_64/IDT.h"
#include "../arch/x86_64/keyboard.h"
#endif

#ifdef __test
#include <minunit.h>
#endif

char *splash =
    "=============================================================================="
    "\nWelcome to\n"
    " /$$      /$$                 /$$  /$$$$$$   /$$$$$$ \n"
    "| $$$    /$$$                | $$ /$$__  $$ /$$__  $$\n"
    "| $$$$  /$$$$  /$$$$$$   /$$$$$$$| $$  \\ $$| $$  \\__/\n"
    "| $$ $$/$$ $$ |____  $$ /$$__  $$| $$  | $$|  $$$$$$ \n"
    "| $$  $$$| $$  /$$$$$$$| $$  | $$| $$  | $$ \\____  $$\n"
    "| $$\\  $ | $$ /$$__  $$| $$  | $$| $$  | $$ /$$  \\ $$\n"
    "| $$ \\/  | $$|  $$$$$$$|  $$$$$$$|  $$$$$$/|  $$$$$$/\n"
    "|__/     |__/ \\_______/ \\_______/ \\______/  \\______/ \n\n";

void kernel_main(void)
{
    terminal_set_theme(VGA_COLOR_BLACK, VGA_COLOR_GREEN);
    terminal_initialize();

#if defined(__x86_64)
    init_IDT();
    init_keyboard();
#endif

    printf("%s\n", splash);

#ifdef __test
    init_test();
#endif

}