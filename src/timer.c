#include "../include/timer.h"

void timer_initialize( timer *timer )
{
	timer->start_ticks = 0;
	timer->paused_ticks = 0;
	
	timer->paused = false;
	timer->started = false;
}

void timer_start( timer *timer )
{
	timer->started = true;
	timer->paused = false;
	
	timer->start_ticks = SDL_GetTicks();
	timer->paused_ticks = 0;
}

void timer_stop( timer *timer )
{
	timer->started = false;
	timer->paused = false;
	
	timer->start_ticks = 0;
	timer->paused_ticks = 0;
}

void timer_pause( timer *timer )
{
	if( timer->started && !timer->paused )
    {
        timer->paused = true;

		timer->paused_ticks = SDL_GetTicks() - timer->start_ticks;
		timer->start_ticks = 0;
    }
}

void timer_unpause( timer *timer )
{
	if( timer->started && timer->paused )
    {
        timer->paused = false;
        
		timer->start_ticks = SDL_GetTicks() - timer->paused_ticks;
		timer->paused_ticks = 0;
    }
}

Uint32 timer_get_ticks( const timer *timer )
{
	Uint32 time = 0;
	
	if ( timer->started ) {
		if ( timer->paused ) {
			time = timer->paused_ticks;
		} else {
			time = SDL_GetTicks() - timer->start_ticks ;
		}
	}
	
	return time;
}

bool timer_is_started( const timer *timer )
{
	return timer->started;
}

bool timer_is_paused( const timer *timer )
{
	return timer->paused && timer->started;
}

void timer_free( timer *timer )
{
	if ( timer != NULL ) {
		puts( "Freeing timer..." );
		free( timer );
	} else {
		puts( "No timer to free!" );
	}
}