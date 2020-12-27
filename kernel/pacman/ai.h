#ifndef __PACMAN_AI_H
#define __PACMAN_AI_H 1

#include "pacman.h"

#include <stdint.h>

void ghost_ai(struct game *state, uint8_t ghost);
void pacman_ai(struct game *state);
void pacman_keyboard(struct game *state);

#endif