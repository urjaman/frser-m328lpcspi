/*
 * This file is part of the frser-m328lpcspi project.
 *
 * Copyright (C) 2009 Urja Rannikko <urjaman@gmail.com>
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

#include "frser-flashapi.h"

#define SPI_DDR                 DDRB
#define SPI_PORT                PORTB

#define SPI_CS                  PB0
#define SPI_DI                  PB3
#define SPI_DO                  PB4
#define SPI_CLK                 PB5

void flash_set_safe(void);
uint8_t flash_get_proto(void);
uint8_t flash_idle_clock(void);
void flash_portclear(void);
