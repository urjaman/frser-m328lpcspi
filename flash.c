/*
 * This file is part of the frser-m328lpcspi project.
 *
 * Copyright (C) 2010,2011 Urja Rannikko <urjaman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#include "main.h"
#include "flash.h"
#include "uart.h"
#include "lpcfwh.h"
#include "spilib.h"
#include "frser.h"
#include "nibble.h"

static uint8_t flash_prot_in_use=0;

void flash_portclear(void) {
	/* This gives both nibble (fwh/lpc) and SPI sane PORT/DDR defaults. */
	/* PORTB:
	 * PB0: SPI CS, OC/EXT PU
	 * (PB2: keep high out so AVR doesnt freak out re:SPI).
	 * PB3: SPI MOSI
	 * PB4: SPI MISO
	 * PB5: SPI SCK / LED */
	/* PORTC:
	 * PC0-3 LPC/FWH D0-D3, OC */
	/* PORTD:
	 * PD0: UART RXD
	 * PD1: UART TXD
	 * PD2: LPC RST, OC
	 * PD3: LPC FRAME
	 * PD4: LPC INIT, OC
	 * PD7 (or hacked PD6 if PD7 is bad to use): LPC CLK */
	PORTB = _BV(1) | _BV(2);
	DDRB = _BV(2) | _BV(3) | _BV(5);
	PORTC = 0;
	DDRC = 0;
	PORTD = _BV(0) | _BV(1) | _BV(3);
	DDRD = _BV(1) | _BV(3);
	CLK_PORT |= _BV(CLK);
	CLK_DDR |= _BV(CLK);
}

void flash_set_safe(void) {
	flash_portclear();
}

//#define SD(x) SEND(X)
#define SD(x)

uint8_t flash_get_proto(void) {
	return flash_prot_in_use;
}

uint8_t flash_idle_clock(void) {
#if 1
	if (flash_prot_in_use&(CHIP_BUSTYPE_LPC|CHIP_BUSTYPE_FWH)) {
		clock_cycle();
		return 1;
	}
#endif
	return 0;
}

void flash_select_protocol(uint8_t allowed_protocols) {
	allowed_protocols &= SUPPORTED_BUSTYPES;
	flash_portclear();
	if ((allowed_protocols&CHIP_BUSTYPE_LPC)&&(lpc_test())) {
		flash_prot_in_use = CHIP_BUSTYPE_LPC;
		return;
	}
	flash_portclear();
	if ((allowed_protocols&CHIP_BUSTYPE_FWH)&&(fwh_test())) {
		flash_prot_in_use = CHIP_BUSTYPE_FWH;
		return;
	}
	flash_portclear();
	/* SPI last because it is really independent of FWH/LPC and works even when not selected. */
	if ((allowed_protocols&CHIP_BUSTYPE_SPI)&&(spi_test())) {
		flash_prot_in_use = CHIP_BUSTYPE_SPI;
		return;
	}
	flash_prot_in_use = 0;
	return;
}


uint8_t flash_read(uint32_t addr) {
	switch (flash_prot_in_use) {
		case 0:
		default:
			return 0xFF;
		case CHIP_BUSTYPE_LPC:
			return lpc_read_address(addr);
		case CHIP_BUSTYPE_FWH:
			return fwh_read_address(addr);
		case CHIP_BUSTYPE_SPI:
			return spi_read(addr);
	}
}

void flash_readn(uint32_t addr, uint32_t len) {
	if (len==0) len = ((uint32_t)1<<24);
	switch (flash_prot_in_use) {
		case 0:
		default:
			while (len--) SEND(0xFF);
			return;
		case CHIP_BUSTYPE_LPC:
			while (len--) SEND(lpc_read_address(addr++));
			return;
		case CHIP_BUSTYPE_FWH:
			while (len--) SEND(fwh_read_address(addr++));
			return;
		case CHIP_BUSTYPE_SPI:
			spi_readn(addr,len);
			return;
	}
}

void flash_write(uint32_t addr, uint8_t data) {
	switch (flash_prot_in_use) {
		case 0:
		default:
			return;
		case CHIP_BUSTYPE_LPC:
			lpc_write_address(addr,data);
			return;
		case CHIP_BUSTYPE_FWH:
			fwh_write_address(addr,data);
			return;
	}
}


void flash_spiop(uint32_t sbytes, uint32_t rbytes) {
	if ((SUPPORTED_BUSTYPES) & CHIP_BUSTYPE_SPI) {
		spi_init_cond();
		spi_spiop(sbytes,rbytes);
		return;
	} else {
		while (sbytes--) RECEIVE();
		SEND(S_ACK);
		while (rbytes--) SEND(0xFF);
		return;
	}
}
