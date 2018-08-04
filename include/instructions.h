#pragma once

#include "s8nchip.h"

void chip_error					( struct chip *cpu 										);
void chip_clear					( struct chip *cpu 										);
void chip_jump					( struct chip* cpu, uint16_t address 					);
void chip_skip					( struct chip* cpu, bool expression 					);
void chip_call					( struct chip* cpu, uint16_t address 					);
void chip_assign_register		( struct chip* cpu, uint8_t register_id, uint8_t value 	);
void chip_add_carry				( struct chip* cpu, uint8_t left, uint8_t right 		);
void chip_subtract_borrow		( struct chip* cpu, uint8_t left, uint8_t right 		);
void chip_shift_left			( struct chip* cpu 										);
void chip_shift_right			( struct chip* cpu 										);
void chip_random				( struct chip* cpu 										);
void chip_draw					( struct chip* cpu 										);
void chip_wait_key_press		( struct chip* cpu 										);
void chip_assign_i				( struct chip* cpu, uint16_t value 						);
void chip_store_bcd				( struct chip* cpu 										);
void chip_copy_to_memory		( struct chip* cpu 										);
void chip_copy_from_memory		( struct chip* cpu 										);
void chip_assign_delay_timer	( struct chip* cpu, uint8_t value 						);
void chip_assign_sound_timer	( struct chip* cpu, uint8_t value 						);