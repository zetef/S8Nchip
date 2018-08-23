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
} Timer ;

void timer_initialize( Timer *timer );
void timer_start( Timer *timer );
void timer_stop( Timer *timer );
void timer_pause( Timer *timer );
void timer_unpause( Timer *timer );
Uint32 timer_get_ticks( Timer *timer );
bool timer_is_started( Timer *timer );
bool timer_is_paused( Timer *timer );