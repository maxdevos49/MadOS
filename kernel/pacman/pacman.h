#ifndef __PACMAN_H
#define __PACMAN_H 1

#include <stdint.h>
#include <graphics.h>

#define MAP_WIDTH (int)28
#define MAP_HEIGHT (int)36

enum ENTITY_TYPE
{
    PACMAN,
    GHOST
};

enum ENTITY_NAME
{
    BLINKY,
    PINKY,
    INKY,
    CLYDE,
    OTHER
};

static const int DIRECTION_OFFSET[] = {-1, 1, 1, -1};
enum DIRECTION
{
    TOP = 0,
    RIGHT,
    BOTTOM,
    LEFT
};

enum GAME_MODE
{
    SCATTER,
    CHASE,
    FRIGHTENED
};

struct entity
{
    enum ENTITY_NAME name;
    //Actual location
    uint32_t x;
    uint32_t y;

    //Target location
    uint32_t x_target;
    uint32_t y_target;

    //Map location
    uint32_t cell_x;
    uint32_t cell_y;

    uint32_t color;
    enum DIRECTION des_dir;
    enum DIRECTION dir;
    enum ENTITY_TYPE type;
};

struct game
{
    uint32_t ticks;
    uint32_t switcher;
    uint16_t score;
    enum GAME_MODE mode;

    struct entity *pacman;
    struct entity *ghost[4];

    //Rendering Details
    GRAPHICS_CONTEXT *ctx;
    uint8_t *map;
    uint32_t scale;
    int32_t screen_width;
    int32_t screen_height;
};

int pacman_main();
void pacman_init(struct game *game);
void pacman_cleanup(struct game *game);

#endif