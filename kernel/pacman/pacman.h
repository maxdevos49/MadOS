#ifndef __PACMAN_H
#define __PACMAN_H 1

#include <stdint.h>
#include <graphics.h>

#define MAP_WIDTH (int)28
#define MAP_HEIGHT (int)36

enum entity_type
{
    PACMAN,
    GHOST
};

enum entity_mode
{
    PACMAN_CONTROL,
    PACMAN_AI,
    GHOST_CHASE,
    GHOST_SCATTER
};

enum entity_direction
{
    UP,
    RIGHT,
    BOTTOM,
    LEFT
};

struct entity
{
    uint32_t x;
    uint32_t y;
    uint32_t color;
    enum entity_direction direction;
    enum entity_type type;
    enum entity_mode mode;
};

struct game
{
    uint32_t ticks;
    uint16_t score;

    struct entity *pacman;
    struct entity *ghost[4];

    uint32_t scale;
    int32_t screen_width;
    int32_t screen_height;

    uint8_t *map;

    GRAPHICS_CONTEXT *ctx;
};

// int pacman_main(int argc, char *argv[]);
int pacman_main();
void pacman_init(struct game *game);
void pacman_cleanup(struct game *game);

#endif