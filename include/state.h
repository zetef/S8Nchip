#pragma once

#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "s8nchip.h"

typedef struct
{
	SDL_Event event;
	bool quit;
} state ;

void state_initialize( state *game_state );
void state_handle( state *game_state, chip *cpu );
void state_free( state *game_state );