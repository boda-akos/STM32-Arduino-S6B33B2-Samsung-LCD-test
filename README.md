# STM32-Arduino-S6B33B3-Samsung-LCD-test
Color stripes and text, some graphics like fillRect. Todo: rewrite and test all graphics functions.

A chinese vendor sells these displays at 0.99$ on Aliexp., it is a S6B33B3 which is significally different from B2.

Not the best quality - technology is 10-15 years old - but could be used in many projects. 

data sheet : http://spurtikus.de/wp-content/uploads/2017/03/controller_S6B33B3A.pdf

Rotation is achieved by registers 0x10 and 0x40 (see table in data sheet p.41)

It is proven that this controller's range is limited to 132 x 132 pixels. Somehow it still can fill the rest of the 
LCD if data is sent continously, but one can't address the LCD area with 0x42 and 0x43 registers reset to 0 
(the ones used for setAddressWindow)  if the 0x83 (131) limit is exceeded and nothing is displayed then.
For example, one can draw a fillRect 131x131 pixels, but (131+1)x131 draws nothing. There must be a workaround for that.
It has nothing to do with Rotate.
