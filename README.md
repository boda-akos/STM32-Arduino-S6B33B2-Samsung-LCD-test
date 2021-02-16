# STM32-Arduino-S6B33B3-Samsung-LCD-test

Adafruit GFX and text support, limited rotation. Most (not all features tested) GFX functions seem to work.

A chinese vendor sells these displays at 0.99$ on Aliexp., controller could be a S6B33B3 or a variant of B2. 

For example, it reacts to 0x10 register D0 bit (B3?). In the data sheet the B3's range is limited to 132x132 pixels

(0x42 0x43 registers range 0-0x83). Somehow it still can address 128x160 pixels if D1 (Y/X) in 0x40 is set right (B2?). 

Not the best contrast - technology is 10-15 years old - but could be used in many projects. 

data sheet : http://spurtikus.de/wp-content/uploads/2017/03/controller_S6B33B3A.pdf

Rotation is achieved by registers 0x10 and 0x40 (see table in data sheet p.41) but only portrait mode text works. 

Text size 0, 1 does not work. Adafruit GFX seems to work. Note that the Adafruit cpp and h files are rewritten, 

the name was kept but do not mix up with the original.  Keep these files in a single directory.

The zip-ed file contains the previous version, it can only do limited graphics.

  PB2   //Reset   PB2 is used for boot, pull down with 100Kohm to GND, good for GPIO 
  PA15  //CS     disableDebugPorts() enables these 3 pins for GPIO use!
  PA14  //RS      _"_
  PA13  //WR      If you use SWD or Jtag, take other pins.
  
  PB8-PB15 Data bus D0-D7

  STM32 IDE : rogerclarkmelbourne on Github
