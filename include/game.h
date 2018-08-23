#pragma once

#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "state.h"
#include "display.h"
#include "s8nchip.h"

typedef struct
{
	state    	*event;
	chip 		*cpu;
	display  	*screen;
} game ;

bool game_initialize( game *self,
					  state *event,
                      display *scrn,
                      chip *cpu );
void game_free( game *self );
void game_close( game *self );