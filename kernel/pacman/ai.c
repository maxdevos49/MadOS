#include "ai.h"
#include "pacman.h"

#include <stdio.h>
#include <math.h>

#define VALID_CELL(map, xCell, yCell) (map[yCell * MAP_WIDTH + xCell] != 1)

void blinky_ai(struct game *game, struct entity *entity);
void pinky_ai(struct game *game, struct entity *entity);
void inky_ai(struct game *game, struct entity *entity);
void clyde_ai(struct game *game, struct entity *entity);

int distance(int x1, int y1, int x2, int y2)
{
    printf("(x2-x1)^2 = x_com :(%f-%f)^2 = %f\n", 0, 0, pow(0 - 0, 2));
    printf("(y2-y1)^2 = y_com :(%f-%f)^2 = %f\n", 4, 1, pow(4 - 1, 2));
    printf("sqrt(x_com + y_com) = ? : sqrt(%f + %f) = %f\n\n", pow(0 - 0, 2), pow(4 - 1, 2), sqrt(pow(4 - 1, 2) + pow(4 - 1, 2)));
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

static int is_decision_time(struct game *game, struct entity *entity)
{
    //done animating?
    if (entity->x == entity->x_target && entity->y == entity->y_target)
    {
        entity->cell_x = entity->x / game->scale;
        entity->cell_y = entity->y / game->scale;

        if (entity->type == PACMAN)
        {
            int cell = game->map[entity->cell_y * MAP_WIDTH + entity->cell_x];
            if (cell == 2)
            {
                game->score++;
            }
            else if (cell == 3)
            {
                game->score += 100;
            }
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
static const char *NAMES[] = {
    "BLINKY",
    "PINKY",
    "INKY",
    "CLYDE",
    "Other"};

void entity_ai(struct game *game, struct entity *entity)
{
    // printf("Distance Test: (0,1),(0,4)=3: %d\n", distance(0, 1, 0, 4));

    if (is_decision_time(game, entity) == 1)
    {
        // printf("Entity: %s | ", NAMES[entity->name]);

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
        }

        //Change the actual direction if possible
        if (change_direction(game, entity, entity->des_dir) == 1)
            change_direction(game, entity, entity->dir);

        entity->des_dir = entity->dir;

        printf("\n");
    }

    //Tween
    int step = game->scale / 6;

    if (entity->x != entity->x_target || entity->y != entity->y_target)
    {

        if (entity->dir == TOP || entity->dir == BOTTOM)
            entity->y += DIRECTION_OFFSET[(int)entity->dir] * step;
        else
            entity->x += DIRECTION_OFFSET[(int)entity->dir] * step;
    }
}

void blinky_ai(struct game *game, struct entity *entity)
{
    int top = (entity->dir != BOTTOM) ? VALID_CELL(game->map, entity->cell_x, entity->cell_y - 1) : 0;
    int bottom = (entity->dir != TOP) ? VALID_CELL(game->map, entity->cell_x, entity->cell_y + 1) : 0;
    int left = (entity->dir != RIGHT) ? VALID_CELL(game->map, entity->cell_x - 1, entity->cell_y) : 0;
    int right = (entity->dir != LEFT) ? VALID_CELL(game->map, entity->cell_x + 1, entity->cell_y) : 0;

    int best = -1;
    int shortest = -1;

    if (top)
    {
        int dis = distance(entity->cell_x, entity->cell_y - 1, game->pacman->cell_x, game->pacman->cell_y);
        best = (shortest < dis) ? best : TOP;
        shortest = (shortest < dis) ? shortest : dis;
        // printf("TOP: %d |", dis);
    }

    if (bottom)
    {
        int dis = distance(entity->cell_x, entity->cell_y + 1, game->pacman->cell_x, game->pacman->cell_y);
        best = (shortest < dis) ? best : BOTTOM;
        shortest = (shortest < dis) ? shortest : dis;
        // printf("BOTTOM: %d |", dis);
    }

    if (left)
    {
        int dis = distance(entity->cell_x - 1, entity->cell_y, game->pacman->cell_x, game->pacman->cell_y);
        best = (shortest < dis) ? best : LEFT;
        shortest = (shortest < dis) ? shortest : dis;
        // printf("LEFT: %d |", dis);
    }

    if (right)
    {
        int dis = distance(entity->cell_x + 1, entity->cell_y, game->pacman->cell_x, game->pacman->cell_y);
        best = (shortest < dis) ? best : RIGHT;
        shortest = (shortest < dis) ? shortest : dis;
        // printf("RIGHT: %d |", dis);
    }

    entity->des_dir = best;
}

void pinky_ai(struct game *game, struct entity *entity)
{
}

void inky_ai(struct game *game, struct entity *entity)
{
}

void clyde_ai(struct game *game, struct entity *entity)
{
}