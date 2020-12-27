#include "pacman.h"
#include "render.h"

#include <graphics.h>
#include <stdlib.h>
#include <string.h>

static char integer_string_output[60];
static char *int_to_str(int64_t value)
{
    uint8_t is_negative = 0;

    if (value < 0)
    {
        is_negative = 1;
        value *= -1;
        integer_string_output[0] = '-';
    }

    uint8_t size = 0;
    uint64_t size_tester = (uint64_t)value;

    while (size_tester / 10 > 0)
    {
        size_tester /= 10;
        size++;
    }

    uint8_t index = 0;
    uint64_t new_value = (uint64_t)value;
    while (new_value / 10 > 0)
    {
        uint8_t remainder = new_value % 10;
        new_value /= 10;
        integer_string_output[is_negative + size - index] = remainder + 48;
        index++;
    }

    uint8_t remainder = new_value % 10;
    integer_string_output[is_negative + size - index] = remainder + 48;
    integer_string_output[is_negative + size + 1] = 0;

    return integer_string_output;
}

static char hex_string_output[17];                 // 8bytes == 16 characters  + null byte = 17 total characters
static const char *HEX_TABLE = "0123456789abcdef"; //This is for simplicity but not really required
static char *hex_to_str(uint64_t value)
{

    if (value == 0)
    {
        hex_string_output[0] = '0';
        hex_string_output[1] = '\0';
        return hex_string_output;
    }

    uint8_t hex_index = 0;
    uint8_t *ptr;

    for (int8_t i = 7; i > -1; i--)
    {
        ptr = ((uint8_t *)&value + i);

        if (*ptr == 0 && hex_index == 0)
        {
            continue;
        }
        else
        {
            hex_string_output[hex_index] = HEX_TABLE[((*ptr & 0xf0) >> 4)];
            hex_index++;

            hex_string_output[hex_index] = HEX_TABLE[(*ptr & 0x0f)];
            hex_index++;
        }
    }

    hex_string_output[hex_index] = 0;

    return hex_string_output;
}

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

    swap_buffer(game->ctx);
}

void render_hud(struct game *game)
{
    draw_text(game->ctx, 10, CHAR_HEIGHT, "HIGH SCORE");
    game->score++;
    draw_text(game->ctx, game->screen_width / 2 - (strlen(int_to_str(game->score)) * CHAR_WIDTH) / 2, CHAR_HEIGHT, int_to_str(game->score));

    set_origin(game->ctx, 0, 0);
    int y = 0;
    draw_text(game->ctx, 0, y, "Screen Width:");
    draw_text(game->ctx, 0, y += CHAR_HEIGHT + 1, int_to_str(game->screen_width));
    // game->screen_width++;
    draw_text(game->ctx, 0, y += CHAR_HEIGHT + 1, "Screen Height:");
    draw_text(game->ctx, 0, y += CHAR_HEIGHT + 1, int_to_str(game->screen_height));
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
