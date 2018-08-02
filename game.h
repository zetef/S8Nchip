#pragma once

#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "s8nchip.h"
#include "display.h"

struct game
{
	struct chip 	*cpu;
	struct display  *screen;
};

bool game_initialize( struct game *self,
                      struct display *scrn,
                      struct chip *cpu );

void game_close( struct game *self );
