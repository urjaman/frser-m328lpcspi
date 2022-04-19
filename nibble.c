/*
	This file was part of bbflash, now frser-m328lpcspi.
	Copyright (C) 2013, Hao Liu and Robert L. Thompson
	Copyright (C) 2013 Urja Rannikko <urjaman@gmail.com>

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
#include "main.h"
#include "nibble.h"

#define delay() asm("nop")
#define swap(x) do { asm volatile("swap %0" : "=r" (x) : "0" (x)); } while(0)


void nibble_set_dir(uint8_t dir) {
	if (!dir) {
		NIBBLE_DDR = 0;
	}
}

uint8_t nibble_read(void) {
	uint8_t rv;
	rv = NIBBLE_PIN & 0xF;
	return rv;
}

static void nibble_write_hi(uint8_t data) {
	swap(data);
	NIBBLE_DDR = (~data) & 0xF;
//	data &= 0xF;
//	while ((NIBBLE_PIN & 0xF) != data);
}

void nibble_write(uint8_t data) {
	NIBBLE_DDR = (~data) & 0xF;
//	data &= 0xF;
//	while ((NIBBLE_PIN & 0xF) != data);
}

#define clock_low() do { CLK_PORT &= ~_BV(CLK); } while(0)
#define clock_high() do { CLK_PORT |= _BV(CLK); } while(0)



bool nibble_init(void) {
	uint8_t i;

	INIT_PORT &= ~_BV(INIT);
	INIT_DDR |= _BV(INIT);

	CLK_DDR |= _BV(CLK);
	CLK_PORT |= _BV(CLK);

	FRAME_DDR |= _BV(FRAME);
	FRAME_PORT |=  _BV(FRAME);

	nibble_set_dir(OUTPUT);
	nibble_write(0);

	for (i = 0; i < 24; i++)
		clock_cycle();
	INIT_DDR &= ~_BV(INIT);
	_delay_us(1); // Let pullup work
	for (i = 0; i < 42; i++)
		clock_cycle();

	return true;
}

void nibble_cleanup(void) {
	CLK_DDR &= ~_BV(CLK);
	FRAME_DDR &= ~_BV(FRAME);
	nibble_set_dir(INPUT);
}

void clocked_nibble_write(uint8_t value) {
	clock_low();
	nibble_write(value);
	clock_high();
}

void clocked_nibble_write_hi(uint8_t value) {
	clock_low();
	nibble_write_hi(value);
	clock_high();
}

uint8_t clocked_nibble_read(void) {
	clock_cycle();
	delay();
	delay();
	return nibble_read();
}

void nibble_start(uint8_t start) {
	FRAME_PORT |= _BV(FRAME);
	nibble_set_dir(OUTPUT);
	clock_high();
	FRAME_PORT &= ~_BV(FRAME);
	nibble_write(start);
	clock_cycle();
	FRAME_PORT |= _BV(FRAME);
}

void nibble_hw_init(void) {
	/* All PORT init in flash_portclear(). */

	/* Kick reset here so lpc/fwh.c doesnt need to know about how it is controlled. */
	RST_DDR |= _BV(RST); //!RST
	RST_PORT &= ~_BV(RST);
	_delay_us(1);
	RST_DDR &= ~_BV(RST);
	_delay_us(1); // slow pullup
}
