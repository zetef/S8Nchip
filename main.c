#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "s8nchip.h"
#include "display.h"
#include "game.h"

int main( int argc, char *argv[] )
{
	if ( argc < 2 || argc > 2 ) {
		puts( "Usage: S8Nchip [ game_name ]" );
		return 1;
	}
	
	struct chip *s8nchip = malloc( sizeof *s8nchip );
	struct display *screen = malloc( sizeof *screen );
	struct game *play = malloc( sizeof *play );
	
    if ( !game_initialize( play, screen, s8nchip ) ) {
        puts( "could not init!" );
    } else {
		chip_free( s8nchip );
		display_free( screen );
		
		srand( time( NULL ) );
        bool quit = false;

        SDL_Event e;

		chip_loadGame( play->cpu, argv[ 1 ] );

        while ( !quit ) {
            chip_emulateCycle( play->cpu );

            while ( SDL_PollEvent( &e ) != 0 ) {
                if ( e.type == SDL_QUIT ) {
                    quit = true;
                } else if ( e.key.type == SDL_KEYDOWN ) {
                    if ( e.key.keysym.sym == SDLK_ESCAPE ) {
                        quit = true;
                    }
                }

                chip_handleInput( play->cpu, &e );
            }

            if ( play->cpu->drawFlag )
            {
                SDL_Rect rect;
                rect.x = 0;
                rect.y = 0;
                rect.w = play->screen->SCREEN_WIDTH;
                rect.h = play->screen->SCREEN_HEIGHT;

                SDL_SetRenderDrawColor( play->screen->renderer, 0x00, 0x00, 0x00, 0xFF );
                SDL_RenderClear( play->screen->renderer );

                SDL_SetRenderDrawColor( play->screen->renderer, 0x00, 0x00, 0x00, 0xFF );
                SDL_Rect clear =
                {
                    rect.x * play->screen->MAGNIFIER, rect.y * play->screen->MAGNIFIER,
                    rect.w * play->screen->MAGNIFIER, rect.h * play->screen->MAGNIFIER
                };
                SDL_RenderFillRect( play->screen->renderer, &clear );

                SDL_SetRenderDrawColor( play->screen->renderer, 0xFF, 0xFF, 0xFF, 0xFF );
                for ( int i = 0; i < play->screen->SCREEN_WIDTH * play->screen->SCREEN_HEIGHT; i++ )
                {
                    uint16_t pos_x = rect.x + i % rect.w;
                    uint16_t pos_y = ( rect.y + i / rect.w ) * play->screen->SCREEN_WIDTH;

                    uint16_t pos = pos_x + pos_y;

                    if ( play->cpu->gfx[ pos ] )
                    {
                        SDL_Rect r =
                        {
                            rect.x * play->screen->MAGNIFIER + ( i % rect.w ) * play->screen->MAGNIFIER,
                            rect.y * play->screen->MAGNIFIER + i / rect.w * play->screen->MAGNIFIER,
                            play->screen->MAGNIFIER,
                            play->screen->MAGNIFIER
                        };
                        SDL_RenderFillRect( play->screen->renderer, &r );
                    }
                }
                SDL_RenderPresent( play->screen->renderer );

                play->cpu->drawFlag = false;
            }
			SDL_Delay( 2 );
		}
	}
	
	game_close( play );

    return 0;
}




