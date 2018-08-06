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
		puts( "Usage: S8Nchip [ game_name ]" );
		return 1;
	}
	struct state *play_state = malloc( sizeof *play_state );
	struct display *screen = malloc( sizeof *screen );
	struct chip *s8nchip = malloc( sizeof *s8nchip );
	struct game *play = malloc( sizeof *play );
	
    if ( !game_initialize( play, play_state, screen, s8nchip ) ) {
        puts( "could not init!" );
    } else {
		chip_free( s8nchip );
		display_free( screen );

		chip_loadGame( play->cpu, argv[ 1 ] );

        while ( !play->play_state->quit ) {
            chip_cycle( play->cpu );

			state_handle( play->play_state, play->cpu );

            if ( play->cpu->drawFlag ) {
				for( int y = 0; y < 32; ++y ) {	
					for( int x = 0; x < 64; ++x ) {
						SDL_Rect rect;
						rect.x = x * play->screen->MAGNIFIER;
						rect.y = y * play->screen->MAGNIFIER;
						rect.w = play->screen->MAGNIFIER;
						rect.h = play->screen->MAGNIFIER;
						
						
						if(play->cpu->gfx[(y*64) + x] == 0) 
							SDL_SetRenderDrawColor( play->screen->renderer, 0, 0, 0, 255 );			
						else 
							SDL_SetRenderDrawColor( play->screen->renderer, 255, 255, 255, 255 );

						SDL_RenderFillRect( play->screen->renderer, &rect );
					}
				}
				SDL_RenderPresent( play->screen->renderer );

                play->cpu->drawFlag = false;
            }
			
			//if (  )
			
			SDL_Delay( 2 );
		}
	}
	
	game_close( play );

    return 0;
}