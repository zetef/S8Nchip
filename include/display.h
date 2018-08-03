#pragma once

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct display
{
	int DEFAULT_DRIVER; // = -1

	int SCREEN_WIDTH; // = x
	int SCREEN_HEIGHT; // = y
	int MAGNIFIER; // = z

	int DISPLAY_WIDTH; // = x * z
	int DISPLAY_HEIGHT; // = y * z

	SDL_Window 		*window;
	SDL_Renderer 	*renderer;
};

void display_initialize( struct display *scrn );
void display_free( struct display *scrn );
