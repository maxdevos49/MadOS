#ifndef __PACMAN_RENDER_H
#define __PACMAN_RENDER_H 1

#include "pacman.h"

void render(struct game *state);

void render_map(struct game *game);
void render_ghost(struct entity *ghost);
void render_pacman(struct entity *pacman);
void render_hud(struct game *game);


#endif