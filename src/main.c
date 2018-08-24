#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "../include/timer.h"
#include "../include/state.h"
#include "../include/display.h"
#include "../include/s8nchip.h"
#include "../include/game.h"

int main( int argc, char *argv[] )
{
	if ( argc < 2 || argc > 2 ) { //only one can be true
		puts( "Usage: S8Nchip [ game name ]" );
		return 1;
	}
	
	state *event 	= malloc( sizeof *event );  
	display *screen	= malloc( sizeof *screen ); 
	chip *s8nchip 	= malloc( sizeof *s8nchip );
	game *play 		= malloc( sizeof *play );   
	
    if ( !game_initialize( play, event, screen, s8nchip ) ) {
        puts( "could not init!" );
    } else {
		state_free( event );
		display_free( screen );
		chip_free( s8nchip );

		chip_loadGame( play->cpu, argv[ 1 ] );
		
		//capping frame
		//int counted_frames = 0;
		//timer *fpsTimer = malloc( sizeof *fpsTimer );
		timer *capTimer = malloc( sizeof *capTimer );
		//timer_initialize( fpsTimer );
		timer_initialize( capTimer );
		//timer_start( fpsTimer );

        while ( !play->event->quit ) {
			timer_start( capTimer );
			
            chip_cycle( play->cpu );
			state_handle( play->event, play->cpu );
			chip_draw( play->cpu, play->screen );
			
			/*float avgFPS = counted_frames / ( timer_get_ticks( fpsTimer ) / 1000.0f ); //the 1000 needs f because it assigns to a float
			if ( avgFPS > 2000000 ) { //the first frames will be a big number
				avgFPS = 0;
			}*/ //just to see the fps
			int frame_ticks = timer_get_ticks( capTimer );
			if ( frame_ticks < play->screen->SCREEN_TICK_PER_FRAME ) {
				SDL_Delay( play->screen->SCREEN_TICK_PER_FRAME - frame_ticks );
			}
			//++counted_frames;
			//printf( "%f\n", avgFPS );
		}
	}

	game_close( play );
    return 0;
}