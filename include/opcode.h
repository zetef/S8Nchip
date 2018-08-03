#pragma once

#include <stdint.h>

union opcode {
	uint16_t instruction;
	
	/*
	struct {
		uint8_t n : 4; // 4-bit constant
		uint8_t y : 4; // register identifier
		uint8_t x : 4; // register identifier
		uint8_t op : 4;
	}  ;
	
	struct {
		uint8_t kk : 8; // 8-bit constant
	}  ;
	
	struct {
		uint16_t addr : 12;
	}  ;
	*/ // if it breaks the way I do it restore this
	
	struct {
		uint8_t n : 4; 
		uint8_t y : 4; 
		uint8_t x : 4; 
		uint8_t op : 4;
	
		uint8_t kk : 8;
		
		uint16_t addr : 12;
	} id ; //for short identificators or identification, whatever
	
};