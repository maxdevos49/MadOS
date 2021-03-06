#include "pacman.h"
#include "render.h"

#include <graphics.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char *DIRECTIONS[] = {
    "TOP",
    "RIGHT",
    "BOTTOM",
    "LEFT"};

const char *MODES[] = {
    "SCATTER",
    "CHASE",
    "FRIGHTENED"};

static void render_map(struct game *game);
static void render_hud(struct game *game);
static void render_pacman(struct game *game, struct entity *pacman);
static void render_ghost(struct game *game, struct entity *ghost);

void render(struct game *game)
{
    start_batch(game->ctx, 0xdead);

    set_fill(game->ctx, 0x00081f3a);
    fill_rect(game->ctx, 0, 0, game->screen_width, game->screen_height);

    render_map(game);

    render_ghost(game, game->ghost[BLINKY]);
    render_ghost(game, game->ghost[PINKY]);
    render_ghost(game, game->ghost[INKY]);
    render_ghost(game, game->ghost[CLYDE]);

    render_pacman(game, game->pacman);

    render_hud(game);

    end_batch(game->ctx, 0xdead);
}

static void render_hud(struct game *game)
{
    char buffer[50];
    sprintf(buffer, "Frame Count:   %05d", game->ticks);
    draw_text(game->ctx, 10, 0, buffer);

    sprintf(buffer, "Current Direction: %s", DIRECTIONS[game->pacman->dir]);
    draw_text(game->ctx, 10, CHAR_HEIGHT, buffer);

    sprintf(buffer, "Mode: %s", MODES[game->mode]);
    draw_text(game->ctx, 10, CHAR_HEIGHT + CHAR_HEIGHT, buffer);

    sprintf(buffer, "Scale: %dpx, Step: %dpx", game->scale, game->scale / 6);
    draw_text(game->ctx, game->scale * 15, 0, buffer);

    sprintf(buffer, "Controls: w,a,s,d,q(quit)");
    draw_text(game->ctx, game->scale * 15, CHAR_HEIGHT, buffer);

    sprintf(buffer, "Pacman X: %dpx, Y: %dpx", game->pacman->x, game->pacman->y);
    draw_text(game->ctx, game->scale * 15, CHAR_HEIGHT + CHAR_HEIGHT, buffer);

    sprintf(buffer, "Score: %05d", game->score);
    draw_text(game->ctx, 10, CHAR_HEIGHT * 39, buffer);
}

static void render_map(struct game *game)
{
    int i, j;

    set_origin(game->ctx, (game->screen_width / 2) - ((MAP_WIDTH * game->scale) / 2), game->scale * 3);
    for (i = 0; i < MAP_HEIGHT; i++)
    {

        for (j = 0; j < MAP_WIDTH; j++)
        {
            int x = j * game->scale;
            int y = i * game->scale;
            int scale = game->scale;

            set_fill(game->ctx, 0x000000);
            fill_rect(game->ctx, x, y, scale, scale);

            switch (game->map[i * MAP_WIDTH + j])
            {
            case 1: //blue
                set_fill(game->ctx, 0x0000ff);
                fill_rect(game->ctx, x, y, scale, scale);
                break;
            case 4:
            case 5:
                set_fill(game->ctx, 0xff00ff);
                fill_rect(game->ctx, x, y, scale, scale);
                break;
            case 2: //black with dots
                set_fill(game->ctx, 0x000000);
                fill_rect(game->ctx, x, y, scale, scale);

                set_fill(game->ctx, 0xffaaaa);
                int pixel_size = scale / 6;
                int pixel_offset = scale / 2 - pixel_size / 2;
                fill_rect(game->ctx, x + pixel_offset, y + pixel_offset, pixel_size, pixel_size);
                break;
            case 3: //power pellets
                set_fill(game->ctx, 0x000000);
                fill_rect(game->ctx, x, y, scale, scale);

                if (game->ticks % 18 > 9)
                {
                    set_fill(game->ctx, 0xffaaaa);
                    int pixels_size = scale / 2;
                    int pixels_offset = scale / 2 - pixels_size / 2;
                    fill_rect(game->ctx, x + pixels_offset, y + pixels_offset, pixels_size, pixels_size);
                }

                break;
            }
        }
    }
}

static void render_ghost(struct game *game, struct entity *ghost)
{
    int color = ghost->color;

    if(game->mode == FRIGHTENED)
    {
        if((game->ticks % 20) > 10)
            color = 0x000000aa;
        else
            color = 0x00C0C0C0;
    }

    set_fill(game->ctx, color);
    fill_rect(game->ctx, ghost->x, ghost->y, game->scale, game->scale);
}

static void render_pacman(struct game *game, struct entity *pacman)
{
    set_fill(game->ctx, 0x00ffff00);
    fill_rect(game->ctx, pacman->x, pacman->y, game->scale, game->scale);
}
