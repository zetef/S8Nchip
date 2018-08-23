#include "../include/timer.h"

void timer_initialize( Timer *timer )
{
	timer->start_ticks = 0;
	timer->paused_ticks = 0;
	
	timer->paused = false;
	timer->started = false;
}

void timer_start( Timer *timer )
{
	timer->started = true;
	timer->paused = false;
	
	timer->start_ticks = SDL_GetTicks();
	timer->paused_ticks = 0;
}

void timer_stop( Timer *timer )
{
	timer->started = false;
	timer->paused = false;
	
	timer->start_ticks = 0;
	timer->paused_ticks = 0;
}

void timer_pause( Timer *timer )
{
	if( timer->started && !timer->paused )
    {
        timer->paused = true;

		timer->paused_ticks = SDL_GetTicks() - timer->start_ticks;
		timer->start_ticks = 0;
    }
}

void timer_unpause( Timer *timer )
{
	if( timer->started && timer->paused )
    {
        timer->paused = false;
        
		timer->start_ticks = SDL_GetTicks() - timer->paused_ticks;
		timer->paused_ticks = 0;
    }
}

Uint32 timer_get_ticks( Timer *timer )
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

bool timer_is_started( Timer *timer )
{
	return timer->started;
}

bool timer_is_paused( Timer *timer )
{
	return timer->paused && timer->started;
}