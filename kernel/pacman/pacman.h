#ifndef __PACMAN_H
#define __PACMAN_H 1

#include <stdint.h>

struct pacman
{
    uint16_t score;
};

int pacman_main(int argc, char *argv[]);

#endif