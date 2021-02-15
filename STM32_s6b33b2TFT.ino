//Mr.Wang init
//databus PB8-15
//TFT chip is s6b33b3 !! 0x10 register -and more- differences to s6b33b2

#include "Adafruit_TFTLCD_8bit_STM32.h" 
Adafruit_TFTLCD_8bit_STM32 tft;
int color[]={0xf100,0x07e0,0x001f,0xffff,YELLOW,MAGENTA};  //bl,gn,rd,wh
int _height=160, _width=128;

void setup() {
  disableDebugPorts();
  tft.begin();
  //pinMode(PA13,OUTPUT); pinMode(PA14,OUTPUT); pinMode(PA15,OUTPUT); 
  //pinMode(PB2,OUTPUT);
  for (byte i=PB8; i<=PB15; i++) pinMode(i,OUTPUT);
  
	Serial.begin(9600); int i=0; 
	while (i<99) Serial.println(i++);		

  //initTFT();
  delay(100);
 digitalWrite(PA15,0);  digitalWrite(PA14,1); 
 for (int i=0; i<128*160; i++) {
  
if (i<2048 ) lcdData(color[0]) ;
if (i<4096 && i>=2048) lcdData(color[1]); 
if (i<8192 && i>=4096) {if (i>(4096 + 10*128) && i<(4096 + 11*128) )  lcdData(color[4]); else lcdData(0);} 
if (i<10240 && i>=8192) lcdData(color[4]);
if (i<12288 && i>=10240) lcdData(color[3]);
if (i<16384 && i>=12288) lcdData(color[5]); //lcdData(random(255)*256+random(255)); 
if (i>=16384) lcdData(color[2]); 
}  

tft.setCursor(30,30); 
tft.setTextColor(BLACK,WHITE);
tft.setTextSize(3);
tft.print("Tolna");

//for (int i=0; i<0x20*0x20; i++) lcdData(random(255)*256+random(255)); 

}


void loop(void){
//tft.fillScreen(YELLOW);
  //tft.setAddrWindow(100,100,120,120);
  //for (int i=0; i<400; i++) lcdData(random(255)*256+random(255)); 
  delay(500);
  };


//==============================================================================================
/*
void wrCmd(int c) {
 digitalWrite(PA15,0); 
  digitalWrite(PA14,0);//RS
  wrBus(c);digitalWrite(PA15,1);
   }
   
void wrData(int c) {
  digitalWrite(PA15,0); 
  digitalWrite(PA14,1);//RS
  wrBus(c);
 digitalWrite(PA15,1);
 }
 */
 void lcdData(int data)
{ digitalWrite(PA15,0);
  digitalWrite(PA14,1);//RS
  wrBuss(data /256);
  wrBuss(data & 0x00ff);
  digitalWrite(PA15,1);
}
 
 
void wrBuss(int b) {
GPIOB->regs->BSRR =0xFF000000 + (b<<8); 
digitalWrite(PA13,LOW);  //WR
delayMicroseconds(10);
digitalWrite(PA13,HIGH);  
  }


  
  /* 

 void initTFT(void){  //COG_C177MVHG_03_H
  digitalWrite(PB2,0); delay(10); digitalWrite(PB2,1);delay(10);
  digitalWrite(PA15,0); //CS
  digitalWrite(PA14,0); //RS
  digitalWrite(PA13,1); //WR  
  
  wrBus(0x2c); delay(10); // LCDWriteCommand( 0x2C);delay_1ms(20);------set OTP mode off---------
  wrBus(0xea);delay(10);
  wrBus2(0x02,0x01);delay(10);//------set internal osc on---------LCDWriteCommand( 0x02);LCDWriteCommand( 0x01); 
  wrBus2(0x26,0x01);delay(10);//------booster1 on---------------LCDWriteCommand( 0x26);LCDWriteCommand( 0x01);
  wrBus2(0x26,0x09);delay(10);//------booster1 on and amp on---------LCDWriteCommand( 0x26);LCDWriteCommand( 0x09);
  wrBus2(0x26,0x0B);delay(10);//------booster2 on-------------LCDWriteCommand( 0x26); LCDWriteCommand( 0x0b);
  wrBus2(0x26,0x0F);delay(10);//------booster3 on-------------LCDWriteCommand( 0x26);LCDWriteCommand( 0x0f);
   
  wrBus2(0x10,0x21);delay(10);//------set sdir=0,duty=1/162 swap=0-----LCDWriteCommand( 0x10);LCDWriteCommand( 0x21);   //RGB//0x23
  wrBus2(0x20,0x01); delay(10);//------booster1 on---------------LCDWriteCommand( 0x20);LCDWriteCommand( 0x0A);  
  wrBus2(0x22,0x11); delay(10); //------bias set to 1/5 --------LCDWriteCommand( 0x22);LCDWriteCommand(  0x11);
  wrBus2(0x24,0x11);delay(10);//------set clock fpck=fose/32(Normal)/fpck=fose/16(partial1)-------LCDWriteCommand( 0x24);LCDWriteCommand( 0x11);
  wrBus2(0x28,0x01);delay(10);//------temp comp ratio -0.05%------ LCDWriteCommand( 0x28);LCDWriteCommand( 0x01);
  wrBus2(0x2A,0xd0);delay(10);//------contrast1 set v1 to 3.757v  max=4v----0x2A,CTRL1 LCDWriteCommand( 0x2a);LCDWriteCommand( 0xBB);   //partial display mode 0
  //wrBus2(0x2B,0x10);delay(10);//------contrast2 set v1 to 3.757v  max=4v--------LCDWriteCommand( 0x2b);LCDWriteCommand( 0x20);   //partial display mode 1
  wrBus2(0x30,0x02);delay(10);//------GSM=00:65K color,DSG=0,SGF=0,SGP=01,SGM=0----- LCDWriteCommand( 0x30);LCDWriteCommand( 0x02);//09
  wrBus2(0x32,0x0e);delay(10);//------row vector type=Diagonal ,INC=111-----LCDWriteCommand( 0x32);LCDWriteCommand( 0x0E);//0e
  wrBus2(0x34,0x89);delay(10);//------frame set FIM=ON,FIP=1FRAME,N-BLOCK=9----- LCDWriteCommand( 0x34);LCDWriteCommand( 0x89);//cd
  wrBus2(0x36,0x00);delay(10);//------Low frequency set off-------LCDWriteCommand( 0x36);LCDWriteCommand( 0x00);
  wrBus2(0x45,0x00);delay(10);//------ram skip area set no skip------LCDWriteCommand( 0x45);LCDWriteCommand(  0x00);
  wrBus2(0x40,0x00);delay(10);//------entry mode set : x addr incr, read modify write off--LCDWriteCommand( 0x40);LCDWriteCommand( 0x00);
  wrBus3(0x42,0x00,159);delay(10);//------x address set from 00 to 159--------LCDWriteCommand( 0x42);LCDWriteCommand( 0x00);LCDWriteCommand( 0x9F);  
  wrBus3(0x43,0x00,127);delay(10);//------y address set from 00 to 127--------LCDWriteCommand( 0x43);LCDWriteCommand( 0x00);LCDWriteCommand( 0x7F);  
  wrBus2(0x55,0x00);delay(10);//------partial display mode off-------LCDWriteCommand( 0x55);LCDWriteCommand( 0x00);
  wrBus2(0x53,0x00);delay(10);//------normal display---------LCDWriteCommand( 0x53);LCDWriteCommand( 0x00);
  wrBus2(0x5A,0x00);delay(10);//-------Scrol lstart line Set------- LCDWriteCommand(0x5A);LCDWriteCommand( 0x00);
  wrBus(0x51);delay(10); //------display on set--------delay_1ms(20);LCDWriteCommand( 0x51);
  digitalWrite(PA15,1); //CS
  digitalWrite(PA14,1); //RS
} 
  


void wrBus2(int d1, int d2) {
wrBus(d1);
wrBus(d2);
  }
  
void wrBus3(int d1, int d2, int d3) {
wrBus(d1);
wrBus(d2);
wrBus(d3);
  }
   
*/


  /*

// Sets the LCD address window 
// Relevant to rect/screen fills and H/V lines.  Input coordinates are
// assumed pre-sorted (e.g. x2 >= x1).

void setAddrWindow(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{ 
  digitalWrite(PA15,0);
  digitalWrite(PA14,0); 
wrBus3(0x42,x1,x2); 
wrBus3(0x43,y1,y2);
digitalWrite(PA15,1);
}

void flood(int color, int len)
{
 for (int i=0; i<len ; i++) lcdData(color);
}

void drawFastHLine(int16_t x, int16_t y, int16_t length, uint16_t color)
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


void drawFastVLine(int16_t x, int16_t y, int16_t length, uint16_t color)
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


void fillRect(int16_t x1, int16_t y1, int16_t w, int16_t h, uint16_t fillcolor)
{
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

void fillScreen(uint16_t color)
{
  
  setAddrWindow(0, 0, _width - 1,  - 1);
  flood(color, _height*_width );
}

void drawPixel(int16_t x, int16_t y, uint16_t color)
{
  // Clip
    if((x < 0) || (y < 0) || (x >= _width) || (y >= _height))    return;
    setAddrWindow(x, y, x+1, y+1);
    lcdData(color); 
    //writeRegister16(0x2C, color);
  
}

void pulseWR(void) {
  digitalWrite(PA13,LOW);  //WR
  delayMicroseconds(10);
  digitalWrite(PA13,HIGH);  
  }

inline void writeCommand(uint16_t c)
{
  CS_ACTIVE_CD_COMMAND;
  wrBus(c>>8);
  wrBus(c);
}

void writeRegister8(uint16_t a, uint8_t d)
{
  writeCommand(a);
  //CD_DATA;
  wrBus(d);
  digitalWrite(PA15,1);
}

void writeRegister16(uint16_t a, uint16_t d)
{
  writeCommand(a);
  //CD_DATA;
  wrBus(d>>8);
  wrBus(d);
  digitalWrite(PA15,1);
}


void writeRegisterPair(uint16_t aH, uint16_t aL, uint16_t d)
{
  writeRegister8(aH, d>>8);
  writeRegister8(aL, d);
}

*/
