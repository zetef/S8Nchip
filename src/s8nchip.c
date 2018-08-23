#include <stdio.h>
#include <stdlib.h>
#include "../include/s8nchip.h"

unsigned char s8nFontset[80] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void chip_initialize( chip *cpu )
{
	memset( cpu->gfx, 0, sizeof( cpu->gfx ) );
	memset( cpu->stack, 0, sizeof( cpu->stack ) );
	memset( cpu->key, 0, sizeof( cpu->key ) );
	memset( cpu->V, 0, sizeof( cpu->V ) );
	memset( cpu->memory, 0, sizeof( cpu->memory ) );
	memcpy( cpu->memory, s8nFontset, sizeof( s8nFontset ) );
	
    cpu->pc          			= INTERPRETER_MEMORY_END;    //application starts/loads at 0x200
    cpu->opcode				    = 0;        				//reset opcode
    cpu->I           			= 0;        			   //reset index register
    cpu->sp          			= 0;                      //reset stack pointer
    cpu->drawFlag    			= true;                  //draw at least once the screen
	cpu->delayTimer  			= 0;                    //reset delay timer
	cpu->soundTimer 			= 0;                   //reset sound timer

	srand( time( NULL ) );
}

bool chip_loadGame( chip *cpu, const char *filename )
{
    printf( "Loading: %s\n", filename );

	// Open file
	FILE *pFile = fopen( filename, "rb" );
	if ( pFile == NULL )
	{
		fputs ( "File error", stderr );
		return false;
	}

	// Check file size
	fseek( pFile , 0 , SEEK_END );
	size_t lSize = ftell( pFile );
	rewind( pFile );
	printf( "Filesize: %d\n", ( int )lSize );

	// Allocate memory to contain the whole file
	char *buffer = ( char* ) malloc( sizeof( char ) * lSize );
	if ( buffer == NULL )
	{
		fputs ( "Memory error", stderr );
		return false;
	}

	// Copy the file into the buffer
	size_t result = fread ( buffer, 1, lSize, pFile );
	if ( result != lSize )
	{
		fputs( "Reading error", stderr );
		return false;
	}

	// Copy buffer to Chip8 memory
	if( ( MAX_MEMORY - INTERPRETER_MEMORY_END ) > lSize )
	{
		for( unsigned int i = 0; i < lSize; ++i )
			cpu->memory[ i + INTERPRETER_MEMORY_END ] = buffer[ i ];
	}
	else
		printf( "Error: ROM too big for memory" );

	// Close file, free buffer
	fclose( pFile );
	free( buffer );

	return true;
}

void chip_handleInput( chip *cpu, SDL_Event *event )
{
    if ( event->key.type == SDL_KEYDOWN )
    {
        switch ( event->key.keysym.sym )
        {
        case SDLK_1: cpu->key[ 0x1 ] = 1; break;
        case SDLK_2: cpu->key[ 0x2 ] = 1; break;
        case SDLK_3: cpu->key[ 0x3 ] = 1; break;
        case SDLK_4: cpu->key[ 0xC ] = 1; break;
        case SDLK_q: cpu->key[ 0x4 ] = 1; break;
        case SDLK_w: cpu->key[ 0x5 ] = 1; break;
        case SDLK_e: cpu->key[ 0x6 ] = 1; break;
        case SDLK_r: cpu->key[ 0xD ] = 1; break;
        case SDLK_a: cpu->key[ 0x7 ] = 1; break;
        case SDLK_s: cpu->key[ 0x8 ] = 1; break;
        case SDLK_d: cpu->key[ 0x9 ] = 1; break;
        case SDLK_f: cpu->key[ 0xE ] = 1; break;
        case SDLK_z: cpu->key[ 0xA ] = 1; break;
        case SDLK_x: cpu->key[ 0x0 ] = 1; break;
        case SDLK_c: cpu->key[ 0xB ] = 1; break;
        case SDLK_v: cpu->key[ 0xF ] = 1; break;
        }
    }

    if ( event->key.type == SDL_KEYUP )
    {
        switch ( event->key.keysym.sym )
        {
        case SDLK_1: cpu->key[ 0x1 ] = 0; break;
        case SDLK_2: cpu->key[ 0x2 ] = 0; break;
        case SDLK_3: cpu->key[ 0x3 ] = 0; break;
        case SDLK_4: cpu->key[ 0xC ] = 0; break;
        case SDLK_q: cpu->key[ 0x4 ] = 0; break;
        case SDLK_w: cpu->key[ 0x5 ] = 0; break;
        case SDLK_e: cpu->key[ 0x6 ] = 0; break;
        case SDLK_r: cpu->key[ 0xD ] = 0; break;
        case SDLK_a: cpu->key[ 0x7 ] = 0; break;
        case SDLK_s: cpu->key[ 0x8 ] = 0; break;
        case SDLK_d: cpu->key[ 0x9 ] = 0; break;
        case SDLK_f: cpu->key[ 0xE ] = 0; break;
        case SDLK_z: cpu->key[ 0xA ] = 0; break;
        case SDLK_x: cpu->key[ 0x0 ] = 0; break;
        case SDLK_c: cpu->key[ 0xB ] = 0; break;
        case SDLK_v: cpu->key[ 0xF ] = 0; break;
        }
    }
}

void chip_fetch( chip *cpu )
{
	cpu->opcode
				= cpu->memory[ cpu->pc ] << 8 | cpu->memory[ cpu->pc + 1 ];//fetch opcode
    //0110101 << 8 = 011010100000000 |
    //                      10111010 =
    //               011010110111010 = opcode :)
}

void chip_update_timers( chip *cpu )
{
	if ( cpu->delayTimer ) cpu->delayTimer--;
	if ( cpu->soundTimer ){ if ( cpu->soundTimer == 1 ) printf( "BEEP\a\n" ); cpu->soundTimer--; }
}

void chip_draw( chip *cpu, display *screen )
{
	if ( cpu->drawFlag ) {
		for( int y = 0; y < 32; ++y ) {	
			for( int x = 0; x < 64; ++x ) {
				SDL_Rect rect;
				rect.x = x * screen->MAGNIFIER;
				rect.y = y * screen->MAGNIFIER;
				rect.w = screen->MAGNIFIER;
				rect.h = screen->MAGNIFIER;
				
				if( cpu->gfx[ ( y * 64 ) + x ] == 0) 
					SDL_SetRenderDrawColor( screen->renderer, 0, 0, 0, 255 );			
				else 
					SDL_SetRenderDrawColor( screen->renderer, 255, 255, 255, 255 );

				SDL_RenderFillRect( screen->renderer, &rect );
			}
		}
		SDL_RenderPresent( screen->renderer );

        cpu->drawFlag = false;
    }
}

void chip_cycle( chip *cpu )
{
	chip_fetch( cpu );
	chip_execute( cpu );
	chip_update_timers( cpu );
}

void chip_execute( chip *cpu )
{
    switch ( cpu->opcode & 0xF000 ) {
        case 0x0000:
            switch ( cpu->opcode & 0x000F ) {
				case 0x0000: //clear screen
					memset( cpu->gfx, 0, sizeof( cpu->gfx ) );
					cpu->drawFlag = true;
					cpu->pc += 2;
					//printf( "00E0 Clear screen\n" );
				break;

				case 0x000E: // return from subroutine
					--cpu->sp;
					cpu->pc = cpu->stack[ cpu->sp ];
					cpu->pc += 2;
					//printf( "00EE Return from subroutine\n" );
				break;

				default:
					printf ("Unknown opcode [0x0000]: 0x%X\n", cpu->opcode);
            }
        break;

        case 0x1000: //jump to location nnn
            cpu->pc = cpu->opcode & 0x0FFF;
			//printf( "1NNN Jump to NNN\n" );
        break;

        case 0x2000: //call subroutine at nnn
            cpu->stack[ cpu->sp ] = cpu->pc;
			++cpu->sp;
            cpu->pc = cpu->opcode & 0x0FFF;
			//printf( "2NNN Call NNN\n" );
		break;

        case 0x3000: //skip instruction if Vx == kk
            if ( cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] == (cpu->opcode & 0x00FF) )
                cpu->pc += 4;
            else
                cpu->pc += 2;
			//printf( "3XNN Skip next instruction if VX == NN\n" );
        break;

        case 0x4000: //skip instruction if Vx != kk
            if ( cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] != ( cpu->opcode & 0x00FF ) )
                cpu->pc += 4;
            else
                cpu->pc += 2;
				//printf( "4XNN Skip next instruction if VX != NN\n" );
        break;

        case 0x5000: //skip instruction if Vx == Vy
            if ( cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] == cpu->V[ ( cpu->opcode & 0x00F0 ) >> 4 ] )
                cpu->pc += 4;
            else
                cpu->pc += 2;
			//printf( "5XY0 Skip next instruction if VX == VY\n" );
        break;

        case 0x6000: //Vx = kk
            cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] = cpu->opcode & 0x00FF;
            cpu->pc += 2;
			//printf( "6XNN Set VX = NN\n" );
        break;

        case 0x7000: //Vx += kk
            cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] += cpu->opcode & 0x00FF;
            cpu->pc += 2;
			//printf( "7XNN Set VX = VX + NN\n" );
        break;

        case 0x8000:
            switch ( cpu->opcode & 0x000F )
            {
                case 0x0000: //what is written down
                    cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] = cpu->V[ ( cpu->opcode & 0x00F0 ) >> 4 ];
                    cpu->pc += 2;
					//printf( "8XY0 Set VX = VY\n" );
                break;

                case 0x0001: //OR
                    cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] |= cpu->V[ ( cpu->opcode & 0x00F0 ) >> 4 ];
                    cpu->pc += 2;
					//printf( "8XY1 Set VX |= VY\n" );
				break;

                case 0x0002: //AND
                    cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] &= cpu->V[ ( cpu->opcode & 0x00F0 ) >> 4 ];
                    cpu->pc += 2;
					//printf( "8XY2 Set VX &= VY\n" );
				break;

                case 0x0003: //XOR
                    cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] ^= cpu->V[ ( cpu->opcode & 0x00F0 ) >> 4 ];
                    cpu->pc += 2;
					//printf( "8XY3 Set VX ^= VY\n" );
                break;

                case 0x0004: //ADD
                    if( cpu->V[ ( cpu->opcode & 0x00F0 ) >> 4 ] > (0xFF - cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] ) )
						cpu->V[ 0xF ] = 1; //carry
					else
						cpu->V[ 0xF ] = 0;
					cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] += cpu->V[ ( cpu->opcode & 0x00F0 ) >> 4 ];
                    cpu->pc += 2;
					//printf( "8XY4 Set VX += VY\n" );
                break;

                case 0x0005: //SUB
                    if( cpu->V[ ( cpu->opcode & 0x00F0 ) >> 4 ] > cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] )
						cpu->V[ 0xF ] = 0; // there is a borrow
					else
						cpu->V[ 0xF ] = 1;
					cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] -= cpu->V[ ( cpu->opcode & 0x00F0 ) >> 4 ];
                    cpu->pc += 2;
					//printf( "8XY5 Set VX -= VY\n" );
                break;

                case 0x0006: //SHR
                    cpu->V[ 0xF ] = cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] & 0x1;
                    cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] >>= 1;
                    cpu->pc += 2;
					//printf( "8XY6 Set VX = VY >> 1\n" );
                break;

                case 0x0007: //SUBN
                    if( cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] > cpu->V[ ( cpu->opcode & 0x00F0 ) >> 4 ] )	// VY-VX
						cpu->V[ 0xF ] = 1; // there is a borrow
					else
						cpu->V[ 0xF ] = 0;
					cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] = cpu->V[ ( cpu->opcode & 0x00F0 ) >> 4 ] - cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ];
                    cpu->pc += 2;
					//printf( "8XY7 Set VX = VY - VX\n" );
                break;

                case 0x000E:
                    cpu->V[ 0xF ] = cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] >> 7;
                    cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] <<= 1;
                    cpu->pc += 2;
					//printf( "8XYE Set VX = VY << 1\n" );
                break;

                default:
					printf ("Unknown opcode [0x8000]: 0x%X\n", cpu->opcode);
            }
        break;

        case 0x9000:
            if ( cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] != cpu->V[ ( cpu->opcode & 0x00F0 ) >> 4 ] )
                cpu->pc += 4;
            else
                cpu->pc += 2;
			//printf( "9XY0 Skip next instruction if VX != VY\n" );
        break;

        case 0xA000:
            cpu->I = cpu->opcode & 0x0FFF;
            cpu->pc += 2;
			//printf( "ANNN Set I = NNN\n" );
        break;

        case 0xB000:
            cpu->pc = (cpu->opcode & 0x0FFF) + cpu->V[ 0 ];
			//printf( "BNNN Set PC = V0 + NNN\n" );
        break;

        case 0xC000:
            cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] = ( rand() % ( 0xFF + 1 ) ) & cpu->opcode & 0x00FF ; //start + ( rand() % (int)( stop - start + 1 ) )
            cpu->pc += 2;
			//printf( "BXNN Set VX = rand() & NN\n" );
        break;

        case 0xD000:
            {
                unsigned short x1 = cpu->V[(cpu->opcode & 0x0F00) >> 8];
                unsigned short y1 = cpu->V[(cpu->opcode & 0x00F0) >> 4];
                unsigned short height = cpu->opcode & 0x000F;
                unsigned short pixel;

                cpu->V[ 0xF ] = 0;

                for ( int yl = 0; yl < height; yl++ )
                {
                    pixel = cpu->memory[ cpu->I + yl ];
                    for(int xl = 0; xl < 8; xl++ )
                        if( ( pixel & ( 0x80 >> xl ) ) != 0 )
                        {
                            if( cpu->gfx[ ( x1 + xl + ( ( y1 + yl ) * 64 ) ) ] == 1)
                                cpu->V[0xF] = 1;
                            cpu->gfx[ x1 + xl + ( ( y1 + yl ) * 64 ) ] ^= 1;
                        }
                }

                cpu->drawFlag = true;
                cpu->pc += 2;
            }
			//printf( "DXYN draw( VX, VY, N )\n" );
        break;

        case 0xE000:
            switch ( cpu->opcode & 0x00FF )
            {
                case 0x009E:
                    if ( cpu->key[ cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] ] == 1 )
                        cpu->pc += 4;
                    else
                        cpu->pc += 2;
					//printf( "EX9E Skip next instruction if key[ VX ] is pressed \n" );
                break;

                case 0x00A1:
                    if ( cpu->key[ cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] ] == 0 )
                        cpu->pc += 4;
                    else
                        cpu->pc += 2;
					//printf( "EXA1 Skip next instruction if key[ VX ] is not pressed \n" );
                break;

                default:
					printf ("Unknown opcode [0xE000]: 0x%X\n", cpu->opcode);
            }
        break;

        case 0xF000:
            switch ( cpu->opcode & 0x00FF )
            {
                case 0x0007:
                    cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] = cpu->delayTimer;
                    cpu->pc += 2;
					//printf( "FX07 Set VX = delay_timer\n" );
                break;

                case 0x000A:
                    {
                        bool keyPress = false;

                        for ( short i = 0; i < 16; i++ )
                            if ( cpu->key[ i ] == 1 )
                            {
                                cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] = i;
                                keyPress = true;
                            }

                        if ( !keyPress )
                            return;

                        cpu->pc += 2;
                    }
					//printf( "FX0A Wait a key input\n" );
                break;

                case 0x0015:
                    cpu->delayTimer = cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ];
                    cpu->pc += 2;
					//printf( "FX15 Set delay_timer = VX\n" );
                break;

                case 0x0018:
                    cpu->soundTimer = cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ];
                    cpu->pc += 2;
					//printf( "FX18 Set sound_timer = VX\n" );
                break;

                case 0x001E:
                    // overflow and set the carry flag
                    if ( cpu->I + cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] > 0xFFF )
                        cpu->V[ 0xF ] = 1;
                    else
                        cpu->V[ 0xF ] = 0;
                    cpu->I += cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ];
                    cpu->pc += 2;
					//printf( "FX1E Set I += VX\n" );
                break;

                case 0x0029:
                    cpu->I = cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] * 0x5;
                    cpu->pc += 2;
					//printf( "FX29 Set I = location of sprite digit VX\n" );
                break;

                case 0x0033:
                    cpu->memory[ cpu->I ]     =   cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] / 100;
                    cpu->memory[ cpu->I + 1 ] = ( cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] / 10 ) % 10;
                    cpu->memory[ cpu->I + 2 ] = ( cpu->V[ ( cpu->opcode & 0x0F00 ) >> 8 ] % 100 ) % 10;
					cpu->pc += 2;
					//printf( "FX33 Store BCD\n" );
				break;

                case 0x0055:
                    for ( short i = 0; i < ( cpu->opcode & 0x0F00 ) >> 8; i++ )
                        cpu->memory[ cpu->I + i ] = cpu->V[ i ];
                    cpu->I += (( cpu->opcode & 0x0F00 ) >> 8) + 1;
                    cpu->pc += 2;
					//printf( "FX55 Store registers\n" );
                break;

                case 0x0065:
                    for ( short i = 0; i < ( cpu->opcode & 0x0F00 ) >> 8; i++ )
                        cpu->V[ i ] = cpu->memory[ cpu->I + i ];
                    cpu->I += (( cpu->opcode & 0x0F00 ) >> 8) + 1;
                    cpu->pc += 2;
					//printf( "FX55 Read registers\n" );
                break;

                default:
					printf ("Unknown opcode [0xF000]: 0x%X\n", cpu->opcode);
            }
        break;

        default:
            printf( "unknown opcode: 0x%X\n", cpu->opcode );
        break;
    }
}

void chip_free( chip *cpu )
{
	if ( cpu != NULL ) {
		puts( "Freeing chip..." );
		free( cpu );
	} else {
		puts( "No chip to free!" );
	}
}