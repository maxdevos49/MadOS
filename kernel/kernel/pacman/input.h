#ifndef __PACMAN_INPUT_H
#define __PACMAN_INPUT_H 1

#include "pacman.h"

//Non positive number means end game
int update_input(struct game *state);
void init_input();
void uninit_input();

#endif