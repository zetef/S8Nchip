#pragma once

#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "state.h"
#include "display.h"
#include "s8nchip.h"

struct game
{
	struct state    *play_state;
	struct chip 	*cpu;
	struct display  *screen;
};

bool game_initialize( struct game *self,
					  struct state *game_state,
                      struct display *scrn,
                      struct chip *cpu );

void game_free( struct game *self );

void game_close( struct game *self );
