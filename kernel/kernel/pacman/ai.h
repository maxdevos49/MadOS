#ifndef __PACMAN_AI_H
#define __PACMAN_AI_H 1

#include "pacman.h"

#include <stdint.h>

void general_ai(struct game * game);

void entity_ai(struct game *game, struct entity *entity);
void entity_reverse(struct game *game, struct entity *entity);

#endif