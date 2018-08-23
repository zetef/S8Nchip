#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "../include/state.h"
#include "../include/display.h"
#include "../include/s8nchip.h"
#include "../include/game.h"

int main( int argc, char *argv[] )
{
	if ( argc < 2 || argc > 2 ) {
		puts( "Usage: S8Nchip [ game name ]" );
		return 1;
	}
	state *event = malloc( sizeof *event );
	display *screen = malloc( sizeof *screen );
	chip *s8nchip = malloc( sizeof *s8nchip );
	game *play = malloc( sizeof *play );
	
    if ( !game_initialize( play, event, screen, s8nchip ) ) {
        puts( "could not init!" );
    } else {
		chip_free( s8nchip );
		display_free( screen );

		chip_loadGame( play->cpu, argv[ 1 ] );

        while ( !play->event->quit ) {
            chip_cycle( play->cpu );

			state_handle( play->event, play->cpu );

			chip_draw( play->cpu, play->screen );
			
			SDL_Delay( 2 );
		}
	}
	
	game_close( play );

    return 0;
}