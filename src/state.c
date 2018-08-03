#include "../include/state.h"

void state_initialize( struct state *game_state )
{
	game_state->quit = false;
}

void state_handle( struct state *game_state, struct chip *cpu )
{
	while ( SDL_PollEvent( &game_state->event ) != 0 ) {
        if ( game_state->event.type == SDL_QUIT ) {
            game_state->quit = true;
        } else if ( game_state->event.key.type == SDL_KEYDOWN ) {
            if ( game_state->event.key.keysym.sym == SDLK_ESCAPE ) {
                game_state->quit = true;
            }
        }

        chip_handleInput( cpu, &game_state->event );
    }
}

void state_free( struct state *game_state )
{
	if ( game_state != NULL ) {
		puts( "Freeing state..." );
		free( game_state );
	} else {
		puts( "No state to free!" );
	}
}