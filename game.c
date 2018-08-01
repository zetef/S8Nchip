#include "game.h"
#include "s8nchip.h"
#include "display.h"

bool game_initialize( struct game *self, 
					  struct display *scrn, 
					  struct chip *cpu )
{
	puts( "Allocating memory for screen and cpu..." );
	printf( "%d bytes for screen...\n", sizeof( sizeof *scrn ) );
	printf( "%d bytes for cpu...\n", sizeof( sizeof *cpu ) );
	
	scrn = malloc( sizeof *scrn );
	cpu = malloc( sizeof *cpu );
	
	if ( scrn == NULL ) {
		puts( "Could not allocate memory for screen..." );
		puts( "Closing..." );
		chip_free( cpu ); //just in case
		return false;
	}
	
	if ( cpu == NULL ) {
		puts( "Could not allocate memory for cpu..." );
		puts( "Closing..." );
		display_free( scrn ); //just in case
		return false;
	}
	
	display_initialize( scrn ); //TODO: add names to displays and chips to know who is allocating or freeing
	chip_initialize( cpu );
	// TODO: error checking
	
	self->screen = malloc( sizeof *self->screen );
	self->cpu = malloc( sizeof *self->cpu );

	
	*self->screen = *scrn; //memcpy( self->screen, scrn, sizeof( *scrn ) );
	*self->cpu = *cpu;     //memcpy( self->cpu, cpu, sizeof( *cpu ) );
	
	//display_copy( scrn, self->screen ); // maybe another day
	//chip_copy( cpu, self->cpu );
	
	display_free( scrn );
	chip_free( cpu );
	
	display_initialize( self->screen );
	chip_initialize( self->cpu );
	// TODO: error checking
	
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "Could not init sdl: %s\n", SDL_GetError() );
        return false;
	} else {
		if ( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
            puts( "Linear texture filtering is not enabled" );
			
		self->screen->window = SDL_CreateWindow( "ok lol",
												 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
												 self->screen->DISPLAY_WIDTH, self->screen->DISPLAY_HEIGHT,
												 SDL_WINDOW_SHOWN );
		
		if ( self->screen->window == NULL ) {
			printf( "Could not create window: %s\n", SDL_GetError() );
            return false;
		} else {
			self->screen->renderer = SDL_CreateRenderer( self->screen->window,
														 self->screen->DEFAULT_DRIVER,
														 SDL_FALSE );
														 
			if ( self->screen->renderer == NULL ) {
				printf( "could not create renderer: %s\n", SDL_GetError() );
                return false;
			}/* else {
                int imgFlags = IMG_INIT_PNG;

                if ( !(IMG_Init( imgFlags ) & imgFlags ) ){
                    printf( "could not init sdl_img: %s\n", IMG_GetError() );
                    success = false;
                }
            }*/
		}
	}
					 
	return true;
}

void game_close( struct game *self )
{
	puts( "Closing..." );
	
	chip_free( self->cpu );
	display_free( self->screen );
		
	SDL_Quit();
}