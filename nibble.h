/*
	This file was part of bbflash, now frser-m328lpcspi.
	Copyright (C) 2013, Hao Liu and Robert L. Thompson

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef NIBBLE_H_
#define NIBBLE_H_
#include "mybool.h"

#define OUTPUT 1
#define INPUT 0

#define RST_DDR				DDRD
#define RST_PORT			PORTD
#define RST				PD2

#define FRAME_DDR			DDRD
#define FRAME_PORT			PORTD
#define FRAME				PD3

#define INIT_DDR			DDRD
#define INIT_PORT			PORTD
#define INIT				PD4

#define CLK_DDR				DDRD
#define CLK_PORT			PORTD
#define CLK				PD7

#define NIBBLE_DDR			DDRC
#define NIBBLE_PORT			PORTC
#define NIBBLE_PIN			PINC

bool nibble_init();
void nibble_cleanup();
void clocked_nibble_write(uint8_t value);
void clocked_nibble_write_hi(uint8_t value);
uint8_t clocked_nibble_read();
void nibble_start(uint8_t start);
void nibble_hw_init(void);
void nibble_set_dir(uint8_t dir);
uint8_t nibble_read(void);
void nibble_write(uint8_t data);
#define clock_cycle() do { CLK_PORT &= ~_BV(CLK); CLK_PORT |= _BV(CLK); } while(0)

#endif /* NIBBLE_H_ */
