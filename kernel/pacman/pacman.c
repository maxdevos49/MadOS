#include "pacman.h"
#include "render.h"
#include "input.h"
#include "ai.h"

#include <stdlib.h>
#include <graphics.h>
#include <stdio.h>

//Need to abstract these to std library calls some time
#include <kernel/interrupts/irq.h>
#include <kernel/timer.h>

static uint8_t map[MAP_HEIGHT * MAP_WIDTH] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
    1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1,
    1, 3, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 3, 1,
    1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
    1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 2, 1,
    1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 2, 1,
    1, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 4, 0, 0, 4, 0, 0, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
    1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1,
    1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1,
    1, 3, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 4, 0, 0, 4, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 3, 1,
    1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1,
    1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1,
    1, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 1,
    1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// int pacman_main(int argc, char *argv[])
int pacman_main()
{

    // if (argv[argc] != NULL)
    //     return 1;

    struct game game;
    pacman_init(&game);

    //game loop
    while (1)
    {
        game.ticks++;

        if (update_input(&game))
            break;

        general_ai(&game);

        //calculate next state
        entity_ai(&game, (&game)->pacman);
        entity_ai(&game, (&game)->ghost[BLINKY]);
        entity_ai(&game, (&game)->ghost[PINKY]);
        entity_ai(&game, (&game)->ghost[INKY]);
        entity_ai(&game, (&game)->ghost[CLYDE]);

        render(&game);

        TIMER_sleep_milliseconds(20);
    }

    pacman_cleanup(&game);

    return 0;
}

void pacman_init(struct game *game)
{
    game->ticks = 0;
    game->switcher = 0;
    game->score = 0;
    game->map = &map;

    game->screen_width = get_screen_width(game->ctx) / 2;
    game->screen_height = get_screen_height(game->ctx);
    game->scale = 18;
    game->mode = SCATTER;
    game->ctx = get_graphics_ctx(DOUBLE, 0, 0, game->screen_width, game->screen_height);

    //pacman
    game->pacman = malloc(sizeof(struct entity));
    game->pacman->name = OTHER;
    game->pacman->x = 13 * game->scale + game->scale / 2;
    game->pacman->y = 26 * game->scale;
    game->pacman->cell_x = 13;
    game->pacman->cell_y = 26;
    game->pacman->x_target = game->pacman->x;
    game->pacman->y_target = game->pacman->y;
    game->pacman->type = PACMAN;
    game->pacman->des_dir = LEFT;
    game->pacman->dir = LEFT;

    //Blinky
    game->ghost[BLINKY] = malloc(sizeof(struct entity));
    game->ghost[BLINKY]->name = BLINKY;
    game->ghost[BLINKY]->color = 0x00ff0000;
    game->ghost[BLINKY]->x = 13 * game->scale;
    game->ghost[BLINKY]->y = 14 * game->scale;
    game->ghost[BLINKY]->cell_x = 13;
    game->ghost[BLINKY]->cell_y = 14;
    game->ghost[BLINKY]->x_target = game->ghost[BLINKY]->x;
    game->ghost[BLINKY]->y_target = game->ghost[BLINKY]->y;
    game->ghost[BLINKY]->type = GHOST;
    game->ghost[BLINKY]->des_dir = RIGHT;
    game->ghost[BLINKY]->dir = RIGHT;

    //Pinky
    game->ghost[PINKY] = malloc(sizeof(struct entity));
    game->ghost[PINKY]->name = PINKY;
    game->ghost[PINKY]->color = 0x00ffc0cb;
    game->ghost[PINKY]->x = 13 * game->scale;
    game->ghost[PINKY]->y = 14 * game->scale;
    game->ghost[PINKY]->cell_x = 13;
    game->ghost[PINKY]->cell_y = 14;
    game->ghost[PINKY]->x_target = game->ghost[PINKY]->x;
    game->ghost[PINKY]->y_target = game->ghost[PINKY]->y;
    game->ghost[PINKY]->type = GHOST;
    game->ghost[PINKY]->des_dir = LEFT;
    game->ghost[PINKY]->dir = LEFT;

    //Inky
    game->ghost[INKY] = malloc(sizeof(struct entity));
    game->ghost[INKY]->name = INKY;
    game->ghost[INKY]->color = 0x0000ffff;
    game->ghost[INKY]->x = 13 * game->scale;
    game->ghost[INKY]->y = 14 * game->scale;
    game->ghost[INKY]->cell_x = 13;
    game->ghost[INKY]->cell_y = 14;
    game->ghost[INKY]->x_target = game->ghost[INKY]->x;
    game->ghost[INKY]->y_target = game->ghost[INKY]->y;
    game->ghost[INKY]->type = GHOST;
    game->ghost[INKY]->des_dir = RIGHT;
    game->ghost[INKY]->dir = RIGHT;

    //CLYDE
    game->ghost[CLYDE] = malloc(sizeof(struct entity));
    game->ghost[CLYDE]->name = CLYDE;
    game->ghost[CLYDE]->color = 0x00ffa500;
    game->ghost[CLYDE]->x = 13 * game->scale;
    game->ghost[CLYDE]->y = 14 * game->scale;
    game->ghost[CLYDE]->cell_x = 13;
    game->ghost[CLYDE]->cell_y = 14;
    game->ghost[CLYDE]->x_target = game->ghost[CLYDE]->x;
    game->ghost[CLYDE]->y_target = game->ghost[CLYDE]->y;
    game->ghost[CLYDE]->type = GHOST;
    game->ghost[CLYDE]->des_dir = RIGHT;
    game->ghost[CLYDE]->dir = RIGHT;

    init_input(game);
}

void pacman_cleanup(struct game *game)
{

    destroy_graphics_ctx(game->ctx);

    free(game->pacman);
    for (int i = 0; i < 4; i++)
        free(game->ghost[i]);

    uninit_input();
}