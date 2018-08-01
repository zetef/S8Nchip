#include "display.h"

void display_initialize( struct display *scrn )
{	
	scrn->DEFAULT_DRIVER = -1;
	
	scrn->SCREEN_WIDTH = 64;
	scrn->SCREEN_HEIGHT = 32;
	scrn->MAGNIFIER = 10;
	
	scrn->DISPLAY_WIDTH = scrn->SCREEN_WIDTH * scrn->MAGNIFIER;
	scrn->DISPLAY_HEIGHT = scrn->SCREEN_HEIGHT * scrn->MAGNIFIER;
}

void display_free( struct display *scrn )
{
	if ( scrn != NULL ) {
		puts( "Freeing display..." );
		
		free( scrn );
	}
}