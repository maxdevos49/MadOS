#include "input.h"

#include <kernel/devices/keyboard.h>

#include <stdlib.h>

static char last;
static void handle_input(uint8_t scan_code, char c)
{
    last = c;
}

int update_input(struct game *game)
{
    if (last == 'q')
        return 1;

    switch (last)
    {
    case 'w':
        game->pacman->des_dir = TOP;
        break;
    case 'a':
        game->pacman->des_dir = LEFT;
        break;
    case 's':
        game->pacman->des_dir = BOTTOM;
        break;
    case 'd':
        game->pacman->des_dir = RIGHT;
        break;
    }

    return 0;
}

int init_input(struct game *game)
{
 
    register_kb_handler(handle_input);
}

void uninit_input()
{
    register_kb_handler(NULL);
}
