#include "ai.h"
#include "pacman.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define VALID_CELL(map, xCell, yCell) (map[((yCell)*MAP_WIDTH) + (xCell)] != 1)
#define MAP_CELL(map, xCell, yCell) (map[((yCell)*MAP_WIDTH) + (xCell)])

static void blinky_ai(struct game *game, struct entity *entity);
static void pinky_ai(struct game *game, struct entity *entity);
static void inky_ai(struct game *game, struct entity *entity);
static void clyde_ai(struct game *game, struct entity *entity);
static int change_direction(struct game *game, struct entity *entity, enum DIRECTION direction);
static int is_decision_time(struct game *game, struct entity *entity);
static int distance(int x1, int y1, int x2, int y2);

void general_ai(struct game *game)
{
    if ((game->mode == SCATTER && game->switcher > 150) || (game->mode == FRIGHTENED && game->switcher > 300))
    {
        for (int i = 0; i < 4; i++)
            entity_reverse(game, (game)->ghost[i]);

        game->mode = CHASE;
        game->switcher = 0;
    }
    else if ((game->mode == CHASE) && game->switcher > 600)
    {
        for (int i = 0; i < 4; i++)
            entity_reverse(game, game->ghost[i]);

        game->mode = SCATTER;
        game->switcher = 0;
    }

    game->switcher++;
}

void entity_reverse(struct game *game, struct entity *entity)
{
    if(game->mode == FRIGHTENED)
        return;

    entity->dir = (entity->dir + 2) % 4;
    entity->x_target = entity->cell_x * game->scale;
    entity->y_target = entity->cell_y * game->scale;
}

void entity_ai(struct game *game, struct entity *entity)
{
    if (is_decision_time(game, entity) == 1)
    {
        switch (entity->name)
        {
        case BLINKY:

            blinky_ai(game, entity);
            break;
        case PINKY:
            pinky_ai(game, entity);
            break;
        case INKY:
            inky_ai(game, entity);
            break;
        case CLYDE:
            clyde_ai(game, entity);
            break;
        case OTHER:
            //na
            break;
        }

        //Change the actual direction if possible
        if (change_direction(game, entity, entity->des_dir) == 1)
            change_direction(game, entity, entity->dir);

        entity->des_dir = entity->dir;
    }

    //Tween
    int step;
    if (game->mode == FRIGHTENED && entity->type != PACMAN)
        step = game->scale / 9;
    else
        step = game->scale / 6;

    if (entity->x != entity->x_target || entity->y != entity->y_target)
    {

        if (entity->dir == TOP || entity->dir == BOTTOM)
            entity->y += DIRECTION_OFFSET[(int)entity->dir] * step;
        else
            entity->x += DIRECTION_OFFSET[(int)entity->dir] * step;
    }
}

static int distance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

static int is_decision_time(struct game *game, struct entity *entity)
{
    //done animating?
    if (entity->x == entity->x_target && entity->y == entity->y_target)
    {
        entity->cell_x = entity->x / game->scale;
        entity->cell_y = entity->y / game->scale;

        //Teleporting
        if (entity->cell_x == 0 && entity->dir == LEFT)
        {
            entity->cell_x = MAP_WIDTH - 1;
            entity->x = entity->cell_x * game->scale;
        }

        if (entity->cell_x == MAP_WIDTH - 1 && entity->dir == RIGHT)
        {
            entity->cell_x = 0;
            entity->x = 0;
        }

        if (entity->type == PACMAN)
        {
            int cell = game->map[entity->cell_y * MAP_WIDTH + entity->cell_x];
            
            if (cell == 2 || cell == 4)
            {
                game->score++;
            }
            else if (cell == 3)
            {
                game->score += 100;

                
                    for (int i = 0; i < 4; i++)
                    {
                        entity_reverse(game, game->ghost[i]);
                    }

                game->mode = FRIGHTENED;
                game->switcher = 0;
            }

            if (cell == 4)
                game->map[entity->cell_y * MAP_WIDTH + entity->cell_x] = 5;
            else
                game->map[entity->cell_y * MAP_WIDTH + entity->cell_x] = 0;
        }

        return 1;
    }

    return 0;
}

static int change_direction(struct game *game, struct entity *entity, enum DIRECTION direction)
{
    int des_cell_x = entity->cell_x;
    int des_cell_y = entity->cell_y;

    if (direction == TOP || direction == BOTTOM)
        des_cell_y += DIRECTION_OFFSET[(int)direction];
    else
        des_cell_x += DIRECTION_OFFSET[(int)direction];

    if (VALID_CELL(game->map, des_cell_x, des_cell_y))
    {
        //Valid
        entity->dir = direction;
        entity->x_target = des_cell_x * game->scale;
        entity->y_target = des_cell_y * game->scale;

        return 0;
    }

    return 1;
}

static void target_position(struct game *game, struct entity *entity, int x, int y)
{
    double short_dis = 696.0;
    double dis = 696.0;

    //TOP
    if (entity->dir != BOTTOM && VALID_CELL(game->map, entity->cell_x, entity->cell_y - 1) && (MAP_CELL(game->map, entity->cell_x, entity->cell_y) != 4) && (MAP_CELL(game->map, entity->cell_x, entity->cell_y) != 5))
    {
        dis = distance(entity->cell_x, entity->cell_y - 1, x, y);
        if (dis < short_dis)
        {
            entity->des_dir = TOP;
            short_dis = dis;
        }
    }

    //BOTTOM
    if (entity->dir != TOP && VALID_CELL(game->map, entity->cell_x, entity->cell_y + 1))
    {
        dis = distance(entity->cell_x, entity->cell_y + 1, x, y);
        if (dis < short_dis)
        {
            entity->des_dir = BOTTOM;
            short_dis = dis;
        }
    }

    //LEFT
    if (entity->dir != RIGHT && VALID_CELL(game->map, entity->cell_x - 1, entity->cell_y))
    {
        dis = distance(entity->cell_x - 1, entity->cell_y, x, y);
        if (dis < short_dis)
        {
            entity->des_dir = LEFT;
            short_dis = dis;
        }
    }

    //RIGHT
    if (entity->dir != LEFT && VALID_CELL(game->map, entity->cell_x + 1, entity->cell_y))
    {
        dis = distance(entity->cell_x + 1, entity->cell_y, x, y);
        if (dis < short_dis)
        {
            entity->des_dir = RIGHT;
            short_dis = dis;
        }
    }
}

static void blinky_ai(struct game *game, struct entity *entity)
{
    if (game->mode == SCATTER)
    {
        target_position(game, entity, MAP_WIDTH - 2, 0);
        return;
    }

    if (game->mode == FRIGHTENED)
    {
        target_position(game, entity, entity->cell_x + (rand() % 10) - 5, entity->cell_y + (rand() % 10) - 5);
        return;
    }

    target_position(game, entity, game->pacman->cell_x, game->pacman->cell_y);
}

static void pinky_ai(struct game *game, struct entity *entity)
{
    if (game->mode == SCATTER)
    {
        target_position(game, entity, 0, 0);
        return;
    }

    if (game->mode == FRIGHTENED)
    {
        target_position(game, entity, entity->cell_x + (rand() % 10) - 5, entity->cell_y + (rand() % 10) - 5);
        return;
    }

    int x_offset = 0;
    int y_offset = 0;

    if (game->pacman->dir == LEFT || game->pacman->dir == RIGHT)
        x_offset = DIRECTION_OFFSET[game->pacman->dir] * 4;
    else
        y_offset = DIRECTION_OFFSET[game->pacman->dir] * 4;

    target_position(game, entity, game->pacman->cell_x + x_offset, game->pacman->cell_y + y_offset);
}

static void inky_ai(struct game *game, struct entity *entity)
{
    if (game->mode == SCATTER)
    {
        target_position(game, entity, MAP_WIDTH, MAP_HEIGHT);
        return;
    }

    if (game->mode == FRIGHTENED)
    {
        target_position(game, entity, entity->cell_x + (rand() % 10) - 5, entity->cell_y + (rand() % 10) - 5);
        return;
    }

    int x_offset = 0;
    int y_offset = 0;

    if (game->pacman->dir == LEFT || game->pacman->dir == RIGHT)
        x_offset = DIRECTION_OFFSET[game->pacman->dir] * 2;
    else
        y_offset = DIRECTION_OFFSET[game->pacman->dir] * 2;

    int x = ((game->pacman->cell_x + x_offset) - game->ghost[BLINKY]->cell_x) * 2;
    int y = ((game->pacman->cell_y + y_offset) - game->ghost[BLINKY]->cell_y) * 2;

    target_position(game, entity, game->ghost[BLINKY]->cell_x + x, game->ghost[BLINKY]->cell_y + y);
}

static void clyde_ai(struct game *game, struct entity *entity)
{
    if (game->mode == SCATTER)
    {
        target_position(game, entity, 0, MAP_HEIGHT);
        return;
    }

    if (game->mode == FRIGHTENED)
    {
        target_position(game, entity, entity->cell_x + (rand() % 10) - 5, entity->cell_y + (rand() % 10) - 5);
        return;
    }

    int dis = distance(entity->cell_x, entity->cell_y, game->pacman->cell_x, game->pacman->cell_y);

    if (dis > 8)
        target_position(game, entity, game->pacman->cell_x, game->pacman->cell_y);
    else
        target_position(game, entity, 0, MAP_HEIGHT);
}