#include "pacman.h"
#include "render.h"

#include <graphics.h>
#include <img/tga.h>

#include <stdlib.h>
#include <string.h>

// extern struct TGA_IMAGE map;

void render(struct game *game)
{
    set_fill(game->ctx, 0x00081f3a);
    fill_rect(game->ctx, 0, 0, game->screen_width, game->screen_height);

    render_map(game);

    //TODO
    // render_ghost(game->ghost[0]);
    // render_ghost(game->ghost[1]);
    // render_ghost(game->ghost[2]);
    // render_ghost(game->ghost[3]);

    // render_pacman(game->pacman);
    fill(game->ctx);

    render_hud(game);

    // draw_tga(game->ctx, &map, 50,50);

    swap_buffer(game->ctx);
}

void render_hud(struct game *game)
{
    draw_text(game->ctx, 10, 0, "Context 3: Pacman Demo");
    draw_text(game->ctx, 10, CHAR_HEIGHT, "HIGH SCORE");
    game->score++;
    // draw_text(game->ctx, game->screen_width / 2 - (strlen(int_to_str(game->score)) * CHAR_WIDTH) / 2, CHAR_HEIGHT, int_to_str(game->score));
}

void render_map(struct game *game)
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
            case 2:
                set_fill(game->ctx, 0x000000);
                fill_rect(game->ctx, x, y, scale, scale);

                set_fill(game->ctx, 0xffaaaa);
                int pixel_size = scale / 6;
                int pixel_offset = scale / 2 - pixel_size / 2;
                fill_rect(game->ctx, x + pixel_offset, y + pixel_offset, pixel_size, pixel_size);
                break;
            case 3:
                set_fill(game->ctx, 0x000000);
                fill_rect(game->ctx, x, y, scale, scale);

                if (game->ticks % 5 > 3)
                {
                    set_fill(game->ctx, 0xffaaaa);
                    int pixels_size = scale / 2;
                    int pixels_offset = scale / 2 - pixels_size / 2;
                    fill_rect(game->ctx, x + pixels_offset, y + pixels_offset, pixels_size, pixels_size);
                }

                break;
            }

            // draw_text(x, y, int_to_str(game->map[i * MAP_WIDTH + j]));
        }
    }
}

// void render_ghost(struct entity *ghost)
// {
//     //TODO
// }

// void render_pacman(struct entity *pacman)
// {
//     //TODO
// }
