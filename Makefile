##
## This file is part of the frser-m328lpcspi project.
##
## Copyright (C) 2010,2011 Urja Rannikko <urjaman@gmail.com>
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
##

PROJECT=frser-m328lpcspi
DEPS=uart.h libfrser/udelay.h main.h lpc.h flash.h fwh.h nibble.h libfrser/frser.h frser-cfg.h Makefile
CIFACE_SOURCES=ciface.c console.c lib.c appdb.c commands.c
SOURCES=main.c uart.c flash.c libfrser/udelay.c libfrser/frser.c lpc.c spi.c fwh.c nibble.c $(CIFACE_SOURCES)
CC=avr-gcc
LD=avr-ld
OBJCOPY=avr-objcopy
MMCU=atmega328p
SERIAL_DEV=/dev/ttyUSB0

#AVRBINDIR=/usr/avr/bin/
AVRDUDECMD=avrdude -p m328p -P $(SERIAL_DEV) -b 115200 -c arduino
# If using avr-gcc < 4.6.0, replace -flto with -combine
CFLAGS=-Ilibfrser -mmcu=$(MMCU) -Os -mcall-prologues -Wl,--relax -fno-inline-small-functions -fno-tree-switch-conversion -frename-registers -g -Wall -W -pipe -flto -fwhole-program -std=gnu99


all: $(PROJECT).out

$(PROJECT).hex: $(PROJECT).out
	$(AVRBINDIR)$(OBJCOPY) -j .text -j .data -O ihex $(PROJECT).out $(PROJECT).hex

$(PROJECT).bin: $(PROJECT).out
	$(AVRBINDIR)$(OBJCOPY) -j .text -j .data -O binary $(PROJECT).out $(PROJECT).bin
 
$(PROJECT).out: $(SOURCES) $(DEPS)
	$(AVRBINDIR)$(CC)  $(CFLAGS) -I./ -o $(PROJECT).out $(SOURCES)
	$(AVRBINDIR)avr-size $(PROJECT).out
	
asm: $(SOURCES) $(DEPS)
	$(AVRBINDIR)$(CC) $(CFLAGS) -S  -I./ -o $(PROJECT).s $(SOURCES)
	

program: $(PROJECT).hex
	$(AVRBINDIR)$(AVRDUDECMD) -U flash:w:$(PROJECT).hex


clean:
	rm -f $(PROJECT).bin
	rm -f $(PROJECT).out
	rm -f $(PROJECT).hex
	rm -f $(PROJECT).s
	rm -f *.o
	
backup:
	$(AVRBINDIR)$(AVRDUDECMD) -U flash:r:backup.bin:r

objdump: $(PROJECT).out
	$(AVRBINDIR)avr-objdump -xdC $(PROJECT).out | less
