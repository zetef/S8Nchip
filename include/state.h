#pragma once

#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "s8nchip.h"

struct state
{
	SDL_Event event;
	bool quit;
};

void state_initialize( struct state *game_state );
void state_handle( struct state *game_state, struct chip *cpu );
void state_free( struct state *game_state );