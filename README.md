# STM32-Arduino-S6B33B3-Samsung-LCD-test
Color stripes and text, some graphics like fillRect. Todo: rewrite and test all graphics functions.

A chinese vendor sells these displays at 0.99$ on Aliexp., it is a S6B33B3 which is significally different from B2.

Not the best quality - technology is 10-15 years old - but could be used in many projects. 

data sheet : http://spurtikus.de/wp-content/uploads/2017/03/controller_S6B33B3A.pdf

Rotation is achieved by registers 0x10 and 0x40 (see table in data sheet p.41)

In the data sheet this controller's range is limited to 132x132 pixels (0x42 0x43 registers range 0-0x83). 
Somehow it still can address 128x160 pixels if D1 (Y/X) in 0x40 is set right.

