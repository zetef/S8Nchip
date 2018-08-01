#include <stdio.h>
#include <stdlib.h>
#include "s8nchip.h"

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

void chip_initialize( struct chip *cpu )
{
    cpu->pc          = 0x200;    //application starts/loads at 0x200
    cpu->opcode      = 0;        //reset opcode
    cpu->I           = 0;        //reset index register
    cpu->sp          = 0;        //reset stack pointer
    cpu->drawFlag    = true;

    for ( int i = 0; i < MAX_GFX; ++i )
    {
        cpu->gfx[ i ] = 0;
    }

    for ( int i = 0; i < 16; ++i )
    {
        cpu->stack[ i ] = 0; //clear the stack
        cpu->key[ i ] = 0;
        cpu->V[ i ] = 0; //clear registers
    }

    for ( int i = 0; i < MAX_MEMORY; ++i )
        cpu->memory[ i ] = 0; //clear the memory

    for ( int i = 0; i < FONTSET_SIZE; ++i ) //for later
        cpu->memory[ i ] = s8nFontset[ i ];

    cpu->delayTimer = cpu->soundTimer = 0; //reset timers
}

bool chip_loadGame( struct chip *cpu, const char *filename )
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
	if( ( MAX_MEMORY - INTERPRETER_MEMORY ) > lSize )
	{
		for( unsigned int i = 0; i < lSize; ++i )
			cpu->memory[ i + INTERPRETER_MEMORY ] = buffer[ i ];
	}
	else
		printf( "Error: ROM too big for memory" );

	// Close file, free buffer
	fclose( pFile );
	free( buffer );

	return true;
}

void chip_handleInput( struct chip *cpu, SDL_Event *event )
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

void chip_emulateCycle( struct chip *cpu )
{
    cpu->opcode = cpu->memory[ cpu->pc ] << 8 | cpu->memory[ cpu->pc + 1 ];//fetch opcode
    //0110101 << 8 = 011010100000000 |
    //                      10111010 =
    //               011010110111010 = opcode :)

    unsigned char x = ( cpu->opcode & 0x0F00 ) >> 8;
    unsigned char y = ( cpu->opcode & 0x00F0 ) >> 4;
    unsigned short nnn = cpu->opcode & 0x0FFF;
    unsigned char kk = cpu->opcode & 0x00FF;

    switch ( cpu->opcode & 0xF000 )
    {
        case 0x0000:
            switch ( cpu->opcode & 0x000F )
            {
            case 0x0000: //clear screen
                for ( int i = 0; i < MAX_GFX; ++i )
                    cpu->gfx[ i ] = 0;
                cpu->drawFlag = true;
                cpu->pc += 2;
            break;

            case 0x000E: // return from subroutine
                cpu->pc = cpu->stack[ --cpu->sp ];
                cpu->pc += 2;
            break;

            default:
                printf ("Unknown opcode [0x0000]: 0x%X\n", cpu->opcode);
            }
        break;

        case 0x1000: //jump to location nnn
            cpu->pc = nnn;
        break;

        case 0x2000: //call subroutine at nnn
            cpu->stack[ cpu->sp ] = cpu->pc;
            ++cpu->sp;
            cpu->pc = nnn;
        break;

        case 0x3000: //skip instruction if Vx == kk
            if ( cpu->V[ x ] == ( kk ) )
                cpu->pc += 4;
            else
                cpu->pc += 2;
        break;

        case 0x4000: //skip instruction if Vx != kk
            if ( cpu->V[ x ] != ( kk ) )
                cpu->pc += 4;
            else
                cpu->pc += 2;
        break;

        case 0x5000: //skip instruction if Vx == Vy
            if ( cpu->V[ x ] == cpu->V[ y ] )
                cpu->pc += 4;
            else
                cpu->pc += 2;
        break;

        case 0x6000: //Vx = kk
            cpu->V[ x ] = kk;
            cpu->pc += 2;
        break;

        case 0x7000: //Vx += kk
            cpu->V[ x ] += kk;
            cpu->pc += 2;
        break;

        case 0x8000:
            switch ( cpu->opcode & 0x000F )
            {
                case 0x0000: //what is written down
                    cpu->V[ x ] = cpu->V[ y ];
                    cpu->pc += 2;
                break;

                case 0x0001: //OR
                    cpu->V[ x ] |= cpu->V[ y ];
                    cpu->pc += 2;
                break;

                case 0x0002: //AND
                    cpu->V[ x ] &= cpu->V[ y ];
                    cpu->pc += 2;
                break;

                case 0x0003: //XOR
                    cpu->V[ x ] ^= cpu->V[ y ];
                    cpu->pc += 2;
                break;

                case 0x0004: //ADD
                    if( cpu->V[ y ] > (0xFF - cpu->V[ x ] ) )
						cpu->V[ 0xF ] = 1; //carry
					else
						cpu->V[ 0xF ] = 0;
					cpu->V[ x ] += cpu->V[ y ];
                    cpu->pc += 2;
                break;

                case 0x0005: //SUB
                    if( cpu->V[ y ] > cpu->V[ x ] )
						cpu->V[ 0xF ] = 0; // there is a borrow
					else
						cpu->V[ 0xF ] = 1;
					cpu->V[ x ] -= cpu->V[ y ];

                    cpu->pc += 2;
                break;

                case 0x0006: //SHR
                    cpu->V[ 0xF ] = cpu->V[ x ] & 0x1;
                    cpu->V[ x ] >>= 1;
                    cpu->pc += 2;
                break;

                case 0x0007: //SUBN
                    if( cpu->V[ x ] > cpu->V[ y ] )	// VY-VX
						cpu->V[ 0xF ] = 0; // there is a borrow
					else
						cpu->V[ 0xF ] = 1;
					cpu->V[ x ] = cpu->V[ y ] - cpu->V[ x ];

                    cpu->pc += 2;
                break;

                case 0x000E:
                    cpu->V[ 0xF ] = cpu->V[ x ] >> 7;
                    cpu->V[ x ] <<= 1;
                    cpu->pc += 2;
                break;

                default:
					printf ("Unknown opcode [0x8000]: 0x%X\n", cpu->opcode);
            }
        break;

        case 0x9000:
            if ( cpu->V[ x ] != cpu->V[ y ] )
                cpu->pc += 4;
            else
                cpu->pc += 2;
        break;

        case 0xA000:
            cpu->I = nnn;
            cpu->pc += 2;
        break;

        case 0xB000:
            cpu->pc = nnn + cpu->V[ 0 ];
        break;

        case 0xC000:
            cpu->V[ x ] = ( rand() % ( 0xFF + 1 ) ) & kk ; //start + ( rand() % (int)( stop - start + 1 ) )
            cpu->pc += 2;
        break;

        case 0xD000:
            {
                unsigned short x = cpu->V[(cpu->opcode & 0x0F00) >> 8];
                unsigned short y = cpu->V[(cpu->opcode & 0x00F0) >> 4];
                unsigned short height = cpu->opcode & 0x000F;
                unsigned short pixel;

                cpu->V[ 0xF ] = 0;

                for ( int yl = 0; yl < height; yl++ )
                {
                    pixel = cpu->memory[ cpu->I + yl ];
                    for(int xl = 0; xl < 8; xl++ )
                        if( ( pixel & ( 0x80 >> xl ) ) != 0 )
                        {
                            if( cpu->gfx[ ( x + xl + ( ( y + yl ) * 64 ) ) ] == 1)
                                cpu->V[0xF] = 1;
                            cpu->gfx[ x + xl + ( ( y + yl ) * 64 ) ] ^= 1;
                        }
                }

                cpu->drawFlag = true;
                cpu->pc += 2;
            }
        break;

        case 0xE000:
            switch ( cpu->opcode & 0x00FF )
            {
                case 0x009E:
                    if ( cpu->key[ cpu->V[ x ] ] != 0 )
                        cpu->pc += 4;
                    else
                        cpu->pc += 2;
                break;

                case 0x00A1:
                    if ( cpu->key[ cpu->V[ x ] ] == 0 )
                        cpu->pc += 4;
                    else
                        cpu->pc += 2;
                break;

                default:
					printf ("Unknown opcode [0xE000]: 0x%X\n", cpu->opcode);
            }
        break;

        case 0xF000:
            switch ( cpu->opcode & 0x00FF )
            {
                case 0x0007:
                    cpu->V[ x ] = cpu->delayTimer;
                    cpu->pc += 2;
                break;

                case 0x000A:
                    {
                        bool keyPress = false;

                        for ( short i = 0; i < 16; i++ )
                            if ( cpu->key[ i ] != 0 )
                            {
                                cpu->V[ x ] = i;
                                keyPress = true;
                            }

                        if ( !keyPress )
                            return;

                        cpu->pc += 2;
                    }
                break;

                case 0x0015:
                    cpu->delayTimer = cpu->V[ x ];
                    cpu->pc += 2;
                break;

                case 0x0018:
                    cpu->soundTimer = cpu->V[ x ];
                    cpu->pc += 2;
                break;

                case 0x001E:
                    // overflow and set the carry flag
                    if ( cpu->I + cpu->V[ x ] > 0xFFF )
                        cpu->V[ 0xF ] = 1;
                    else
                        cpu->V[ 0xF ] = 0;
                    cpu->I += cpu->V[ x ];
                    cpu->pc += 2;
                break;

                case 0x0029:
                    cpu->I = cpu->V[ x ] * 0x5;
                    cpu->pc += 2;
                break;

                case 0x0033:
                    cpu->memory[ cpu->I ]     = cpu->V[ x ] / 100;
                    cpu->memory[ cpu->I + 1 ] = ( cpu->V[ x ] / 10 ) % 10;
                    cpu->memory[ cpu->I + 2 ] = ( cpu->V[ x ] % 100 ) % 10;
                break;

                case 0x0055:
                    for ( short i = 0; i < ( ( cpu->opcode & 0x0F00 ) >> 8 ); i++ )
                        cpu->memory[ cpu->I + i ] = cpu->V[ i ];
                    cpu->I += ( ( cpu->opcode & 0x0F00 ) >> 8 ) + 1;
                    cpu->pc += 2;
                break;

                case 0x0065:
                    for ( short i = 0; i < ( ( cpu->opcode & 0x0F00 ) >> 8 ); i++ )
                        cpu->V[ i ] = cpu->memory[ cpu->I + i ];
                    cpu->I += ( ( cpu->opcode & 0x0F00 ) >> 8 ) + 1;
                    cpu->pc += 2;
                break;

                default:
					printf ("Unknown opcode [0xF000]: 0x%X\n", cpu->opcode);
            }
        break;

        default:
            printf( "unknown opcode: 0x%X\n", cpu->opcode );
            cpu->pc += 2;
        break;
    }

    if ( cpu->delayTimer > 0 )
        --cpu->delayTimer;

    if ( cpu->soundTimer > 0 )
    {
        if ( cpu->soundTimer == 1 )
            puts( "BEEP!\a\n" );
        --cpu->soundTimer;
    }
}

void chip_free( struct chip *cpu )
{
	if ( cpu != NULL ) {
		puts( "Freeing chip..." );
		free( cpu );
	}
}


































