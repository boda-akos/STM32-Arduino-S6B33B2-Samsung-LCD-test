# STM32-Arduino-S6B33B3-Samsung-LCD-test

Adafruit GFX support and text, limited rotation. Some but not all GFX functions tested and work well.

A chinese vendor sells these displays at 0.99$ on Aliexp., controller could be a S6B33B3 or a variant of B2. 

For example, it reacts to 0x10 register D0 bit (B3?). In the data sheet the B3's range is limited to 132x132 pixels

(0x42 0x43 registers range 0-0x83). Somehow it still can address 128x160 pixels if D1 (Y/X) in 0x40 is set right (B2?). 

Not the best contrast - technology is 10-15 years old - but could be used in many projects. 

data sheet : http://spurtikus.de/wp-content/uploads/2017/03/controller_S6B33B3A.pdf

Rotation is achieved by registers 0x10 and 0x40 (see table in data sheet p.41) but only portrait mode text works.

Update 16 Feb 2021 : Adafruit GFX seems to work. Note that the Adafruit cpp and h files are rewritten, the name was kept.

The zip-ed file contains the previous version, it can only do limited graphics.



