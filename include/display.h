#pragma once

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "timer.h"

typedef struct
{
	int DEFAULT_DRIVER; // = -1

	int SCREEN_WIDTH; // = x
	int SCREEN_HEIGHT; // = y
	int MAGNIFIER; // = z

	int DISPLAY_WIDTH; // = x * z
	int DISPLAY_HEIGHT; // = y * z

	int SCREEN_FPS;
	int SCREEN_TICK_PER_FRAME; //1000 / SCREEN_FPS

	SDL_Window 		*window;
	SDL_Renderer 	*renderer;
} display ;

void display_initialize( display *scrn );
void display_free( display *scrn );