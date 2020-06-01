#include "../include/display.h"

void display_initialize( display *scrn )
{	
	scrn->DEFAULT_DRIVER = -1;
	
	scrn->SCREEN_WIDTH = 64;
	scrn->SCREEN_HEIGHT = 32;
	scrn->MAGNIFIER = 10;
	
	scrn->DISPLAY_WIDTH = scrn->SCREEN_WIDTH * scrn->MAGNIFIER;
	scrn->DISPLAY_HEIGHT = scrn->SCREEN_HEIGHT * scrn->MAGNIFIER;
	
	scrn->SCREEN_FPS = 360; //360 is pretty good, but 60 was the original
	scrn->SCREEN_TICK_PER_FRAME = 1000 / scrn->SCREEN_FPS;
}

void display_free( display *scrn )
{
	if ( scrn != NULL ) {
		puts( "Freeing display..." );
		free( scrn );
	} else {
		puts( "No dispaly to free!" );
	}
}
