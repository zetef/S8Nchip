#pragma once

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
	Uint32 start_ticks;
	Uint32 paused_ticks;
	
	bool paused;
	bool started;
} timer ;

void timer_initialize( timer *timer );
void timer_start( timer *timer );
void timer_stop( timer *timer );
void timer_pause( timer *timer );
void timer_unpause( timer *timer );
Uint32 timer_get_ticks( const timer *timer );
bool timer_is_started( const timer *timer );
bool timer_is_paused( const timer *timer );
void timer_free( timer *timer );