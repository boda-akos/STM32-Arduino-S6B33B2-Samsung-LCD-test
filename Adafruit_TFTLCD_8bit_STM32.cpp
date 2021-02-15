// Graphics library by ladyada/adafruit with init code from Rossum
// MIT license

#include "Adafruit_TFTLCD_8bit_STM32.h"
#include "s6b.h"

gpio_reg_map * cntrlRegs;
gpio_reg_map * dataRegs;

/*****************************************************************************/
// Constructor
/*****************************************************************************/
Adafruit_TFTLCD_8bit_STM32 :: Adafruit_TFTLCD_8bit_STM32(void)
: Adafruit_GFX(TFTWIDTH, TFTHEIGHT)
{
}

//=======================================
void Adafruit_TFTLCD_8bit_STM32::begin()
{
	reset();
//  driver = ID_s6b;
  s6b_begin();
}

//=======================================
void Adafruit_TFTLCD_8bit_STM32::reset(void)
{
	cntrlRegs = TFT_CNTRL_PORT->regs;
	dataRegs = TFT_DATA_PORT->regs;
	     //Set control lines as output
	     //cntrlRegs->CRL = (cntrlRegs->CRL & 0xFFFF0000) | 0x00003333;
	pinMode(TFT_RD, OUTPUT);
	pinMode(TFT_WR, OUTPUT);
	pinMode(TFT_RS, OUTPUT);
	pinMode(TFT_CS, OUTPUT);
	CS_IDLE; // Set all control bits to HIGH (idle)
	CD_DATA; // Signals are ACTIVE LOW
	WR_IDLE;
	RD_IDLE;
	
	setWriteDir();//set up 8 bit parallel port to write mode.
  	pinMode(TFT_RST, OUTPUT);
		digitalWrite(TFT_RST, HIGH);
		delay(10);
		digitalWrite(TFT_RST, LOW);
		delay(10);
		digitalWrite(TFT_RST, HIGH);
		delay(10);
}

//=======================================
// Sets the LCD address window (and address counter, on 932X).
// Relevant to rect/screen fills and H/V lines.  Input coordinates are
// assumed pre-sorted (e.g. x2 >= x1).
//=======================================
void Adafruit_TFTLCD_8bit_STM32::setAddrWindow(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
  CS_ACTIVE_CD_COMMAND;
 
  write8(0x42);
  write8(x1);
  write8(x2);
  write8(0x43);
  write8(y1);
  write8(y2);
CS_IDLE;
	
}

//=======================================
// Fast block fill operation for fillScreen, fillRect, H/V line, etc.
// Requires setAddrWindow() has previously been called to set the fill
// bounds.  'len' is inclusive, MUST be >= 1.
//=======================================
void Adafruit_TFTLCD_8bit_STM32::flood(uint16_t color, uint32_t len)
{
  CS_ACTIVE;
  CD_DATA;
  for (int i=0; i<len; i++) { 
    write8(color >> 8);
    write8(color & 0x00ff);}
  /*while(len) {
  // Write pixel normally, decrement counter by 1
  CD_DATA;
 
  len--;
  }*/
  CS_IDLE;
}

//=======================================
void Adafruit_TFTLCD_8bit_STM32::drawFastHLine(int16_t x, int16_t y, int16_t length, uint16_t color)
{
  int16_t x2;

  // Initial off-screen clipping
  if((length <= 0     ) ||
     (y      <  0     ) || ( y                  >= _height) ||
     (x      >= _width) || ((x2 = (x+length-1)) <  0      )) return;

  if(x < 0) {        // Clip left
    length += x;
    x       = 0;
  }
  if(x2 >= _width) { // Clip right
    x2      = _width - 1;
    length  = x2 - x + 1;
  }

  setAddrWindow(x, y, x2, y);
  flood(color, length);
   setAddrWindow(0, 0, _width - 1, _height - 1);
  
}

//=======================================
void Adafruit_TFTLCD_8bit_STM32::drawFastVLine(int16_t x, int16_t y, int16_t length, uint16_t color)
{
  int16_t y2;

  // Initial off-screen clipping
  if((length <= 0      ) ||
     (x      <  0      ) || ( x                  >= _width) ||
     (y      >= _height) || ((y2 = (y+length-1)) <  0     )) return;
  if(y < 0) {         // Clip top
    length += y;
    y       = 0;
  }
  if(y2 >= _height) { // Clip bottom
    y2      = _height - 1;
    length  = y2 - y + 1;
  }

  setAddrWindow(x, y, x, y2);
  flood(color, length);
 setAddrWindow(0, 0, _width - 1, _height - 1);
  
}

void Adafruit_TFTLCD_8bit_STM32::fillRect(int16_t x1, int16_t y1, int16_t w, int16_t h, uint16_t fillcolor)
{
	//Serial.println("\n::fillRect...");
  int16_t  x2, y2;

  // Initial off-screen clipping
  if( (w            <= 0     ) ||  (h             <= 0      ) ||
      (x1           >= _width) ||  (y1            >= _height) ||
     ((x2 = x1+w-1) <  0     ) || ((y2  = y1+h-1) <  0      )) return;
  if(x1 < 0) { // Clip left
    w += x1;
    x1 = 0;
  }
  if(y1 < 0) { // Clip top
    h += y1;
    y1 = 0;
  }
  if(x2 >= _width) { // Clip right
    x2 = _width - 1;
    w  = x2 - x1 + 1;
  }
  if(y2 >= _height) { // Clip bottom
    y2 = _height - 1;
    h  = y2 - y1 + 1;
  }

  setAddrWindow(x1, y1, x2, y2);
  flood(fillcolor, (uint32_t)w * (uint32_t)h);
   setAddrWindow(0, 0, _width - 1, _height - 1);

}

//=======================================
void Adafruit_TFTLCD_8bit_STM32::fillScreen(uint16_t color)
{
 
    setAddrWindow(0, 0, _width - 1, _height - 1);
    CS_ACTIVE; CD_DATA;
  for (int i=0; i<128*160; i++) { 
    write8(color >> 8);
    write8(color & 0x00ff);}
  //flood(color,);
  CS_IDLE;
}

//=======================================
void Adafruit_TFTLCD_8bit_STM32::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  // Clip
  if((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return;

    setAddrWindow(x, y, x+1, y+1);
    write8 (color >>8);
    write8 (color & 0x00ff);
}

//=======================================
// Draw an image bitmap (16bits per color) at the specified position from the provided buffer.
//=======================================
void Adafruit_TFTLCD_8bit_STM32::drawBitmap(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t * bitmap)
{
	if ( x>=0 && (x+w)<_width && y>=0 && (y+h)<=_height ) {
		// all pixel visible, do it in the fast way
		setAddrWindow(x,y,x+w-1,y+h-1);
		pushColors((uint16_t*)bitmap, w*h, true);
	} else {
		// some pixels outside visible area, do it in the classical way to disable off-screen points
		int16_t i, j;
		uint16_t * colorP = (uint16_t*)bitmap;
		for(j=0; j<h; j++) {
			for(i=0; i<w; i++ ) {
				drawPixel(x+i, y+j, *colorP++);
			}
		}
	}
}

//=======================================
// Issues 'raw' an array of 16-bit color values to the LCD; used
// externally by BMP examples.  Assumes that setWindowAddr() has
// previously been set to define the bounds.  Max 255 pixels at
// a time (BMP examples read in small chunks due to limited RAM).
//=======================================
void Adafruit_TFTLCD_8bit_STM32::pushColors(uint16_t *data, int16_t len, boolean first)
{
  uint16_t color;
  uint8_t  hi, lo;
  CS_ACTIVE;
  if(first == true) { // Issue GRAM write command only on first call
    CD_COMMAND;
     write8(0x00);
    }
  CD_DATA;
  while(len--) {
    color = *data++;
    hi    = color >> 8; // Don't simplify or merge these
    lo    = color;      // lines, there's macro shenanigans
    write8(hi);         // going on.
    write8(lo);
  }
  CS_IDLE;
}

//=======================================
void Adafruit_TFTLCD_8bit_STM32::invertDisplay(boolean i)
{
		//writeCommand( i ? 0 : 1);
		CS_IDLE;
}
//=======================================
void Adafruit_TFTLCD_8bit_STM32::setRotation(uint8_t x){
  // Call parent rotation func first -- sets up rotation flags, etc.
  Adafruit_GFX::setRotation(x);
  // Then perform hardware-specific rotation operations...
/*
      {
    // Like HX8357D, but mirrored
    uint16_t t;
    
    switch (rotation) {
      case 1:
        t = HX8357B_MADCTL_MY | HX8357B_MADCTL_MX | HX8357B_MADCTL_MV | HX8357B_MADCTL_RGB;
        break;
      case 2:
        t = HX8357B_MADCTL_MY | HX8357B_MADCTL_RGB;
        break;
      case 3:
        t = HX8357B_MADCTL_MV | HX8357B_MADCTL_RGB;
        break;
      case 0:
      default:
        t = HX8357B_MADCTL_MH | HX8357B_MADCTL_MX | HX8357B_MADCTL_RGB;
        break;
    }*/
    //writeRegister8(ILI9341_MADCTL, t ); // MADCTL
    //default full-screen address window:
    setAddrWindow(0, 0, _width - 1, _height - 1); // CS_IDLE happens here
  
}

//=======================================
uint8_t read8_(void)
{
  RD_ACTIVE;
  delayMicroseconds(10);
  uint8_t temp = ( (dataRegs->IDR>>TFT_DATA_SHIFT) & 0x00FF);
  delayMicroseconds(10);
  RD_IDLE;
  delayMicroseconds(10);
  return temp;
}


/*
// Because this function is used infrequently, it configures the ports for
// the read operation, reads the data, then restores the ports to the write
// configuration.  Write operations happen a LOT, so it's advantageous to
// leave the ports in that state as a default.

uint16_t Adafruit_TFTLCD_8bit_STM32::readPixel(int16_t x, int16_t y)
{
  if((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return 0;
  return 0;
}

uint16_t Adafruit_TFTLCD_8bit_STM32::readID(void) {
  return (0x6b);
}


uint32_t readReg32(uint8_t r)
{
  uint32_t id;
  uint8_t x;

  // try reading register #4
  writeCommand(r);
  setReadDir();  // Set up LCD data port(s) for READ operations
  CD_DATA;
  delayMicroseconds(50);
  read8(x);
  id = x;          // Do not merge or otherwise simplify
  id <<= 8;              // these lines.  It's an unfortunate
  read8(x);
  id  |= x;        // shenanigans that are going on.
  id <<= 8;              // these lines.  It's an unfortunate
  read8(x);
  id  |= x;        // shenanigans that are going on.
  id <<= 8;              // these lines.  It's an unfortunate
  read8(x);
  id  |= x;        // shenanigans that are going on.
  CS_IDLE;
  setWriteDir();  // Restore LCD data port(s) to WRITE configuration
  return id;
}
uint16_t readReg(uint8_t r)
{
  uint16_t id;
  uint8_t x;

  writeCommand(r);
  setReadDir();  // Set up LCD data port(s) for READ operations
  CD_DATA;
  delayMicroseconds(10);
  read8(x);
  id = x;          // Do not merge or otherwise simplify
  id <<= 8;              // these lines.  It's an unfortunate
  read8(x);
  id |= x;        // shenanigans that are going on.
  CS_IDLE;
  setWriteDir();  // Restore LCD data port(s) to WRITE configuration

  //Serial.print("Read $"); Serial.print(r, HEX); 
  //Serial.print(":\t0x"); Serial.println(id, HEX);
  return id;
}
****/
inline void writeCommand(uint16_t c)
{
  CS_ACTIVE_CD_COMMAND;
  write8(c>>8);
  write8(c);
}

//=======================================
void writeRegister8(uint16_t a, uint8_t d)
{
  writeCommand(a);
  //CD_DATA;
  write8(d);
  CS_IDLE;
}

//=======================================
void writeRegister16(uint16_t a, uint16_t d)
{
  writeCommand(a);
  CD_DATA;
  write8(d>>8);
  write8(d);
  CS_IDLE;
}

//=======================================
void writeRegisterPair(uint16_t aH, uint16_t aL, uint16_t d)
{
  writeRegister8(aH, d>>8);
  writeRegister8(aL, d);
}

//=======================================
void writeRegister24(uint16_t r, uint32_t d)
{
  writeCommand(r); // includes CS_ACTIVE
  CD_DATA;
  write8(d >> 16);
  write8(d >> 8);
  write8(d);
  CS_IDLE;
}

//=======================================
void writeRegister32(uint16_t r, uint32_t d)
{
  writeCommand(r);
  CD_DATA;
  write8(d >> 24);
  write8(d >> 16);
  write8(d >> 8);
  write8(d);
  CS_IDLE;
}

//Adafruit_TFTLCD_8bit_STM32 tft;
