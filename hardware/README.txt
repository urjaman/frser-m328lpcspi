There was no schematic, it was done in EAGLE board editor,
but now i made a schematic just so you can see the connections...
Because apparently people cant read a board :| .
Also exported a schematic.png

The basic connections can be gleaned from flash.c
or with EAGLE and also the resistor values can be had from
eagle, but i'll write them here for faster reference

around SPI:
leftmost: CS pullup resistor 4k7
resistors near board top edge (above) 2k2
resistor below them 4k7

below PLCC32 socket:
leftmost 4: 2k2 pullup for data lines
next 2: 4k7 pullup for RST and INIT
next 4 (oriented left-right):
the ones near board edge (below) are 4k7
the ones above them are 2k2


The .zip contains the gerber files, or you 
can see:
https://oshpark.com/shared_projects/E6jwmbWy
