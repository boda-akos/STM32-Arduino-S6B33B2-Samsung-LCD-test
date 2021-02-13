//Mr.Wang init
//databus PB8-15


int color[]={0xf100,0x07e0,0x001f,0xffff};  //bl,gn,rd,wh


void setup() {
  disableDebugPorts();
  pinMode(PA13,OUTPUT); pinMode(PA14,OUTPUT); pinMode(PA15,OUTPUT); 
  pinMode(PB2,OUTPUT);
  for (byte i=PB8; i<=PB15; i++) pinMode(i,OUTPUT);
  
	Serial.begin(9600); int i=0; 
	while (i<99) Serial.println(i++);		

  initTFT();
  delay(100);
 digitalWrite(PA15,0);  digitalWrite(PA14,1); 
 for (int i=0; i<128*160; i++) {
  
if (i<2048 ) lcdData(color[0]) ;
if (i<4096 && i>=2048) lcdData(color[1]); 
if (i<8192 && i>=4096) {if (i>(4096 + 10*128) && i<(4096 + 11*128) )  lcdData(color[0]); else lcdData(0);} 
if (i<16384 && i>=8192) lcdData(color[3]); //lcdData(random(255)*256+random(255)); 
if (i>=16384) lcdData(color[2]); 
}  digitalWrite(PA15,1);
 
 digitalWrite(PA15,0);
 //wrBus(0x42); wrBus(100); wrBus(101); wrBus(0x43); wrBus(100); wrBus(101);
 digitalWrite(PA15,1);
 //wrData(0);
}

void loop() {
  //if ( ! mfrc522.PICC_IsNewCardPresent()) return;if ( ! mfrc522.PICC_ReadCardSerial()) return;mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
 //for (int i=0; i<256; i++) wrBus(i);
}

void wrBus2(int d1, int d2) {
wrBus(d1);
wrBus(d2);
  }
   
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
   
void wrBus(int b) {
GPIOB->regs->BSRR =0xFF000000 + (b<<8); 
digitalWrite(PA13,LOW);  //WR
delayMicroseconds(10);
digitalWrite(PA13,HIGH);  
  }

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
  wrBus(0x42);wrBus(0x00);wrBus(159);delay(10);//------x address set from 00 to 159--------LCDWriteCommand( 0x42);LCDWriteCommand( 0x00);LCDWriteCommand( 0x9F);  
  wrBus(0x43);wrBus(0x00);wrBus(127);delay(10);//------y address set from 00 to 127--------LCDWriteCommand( 0x43);LCDWriteCommand( 0x00);LCDWriteCommand( 0x7F);  
  wrBus2(0x55,0x00);delay(10);//------partial display mode off-------LCDWriteCommand( 0x55);LCDWriteCommand( 0x00);
  wrBus2(0x53,0x00);delay(10);//------normal display---------LCDWriteCommand( 0x53);LCDWriteCommand( 0x00);
  wrBus2(0x5A,0x00);delay(10);//-------Scrol lstart line Set------- LCDWriteCommand(0x5A);LCDWriteCommand( 0x00);
  wrBus(0x51);delay(10); //------display on set--------delay_1ms(20);LCDWriteCommand( 0x51);
  digitalWrite(PA15,1); //CS
  digitalWrite(PA14,1); //RS
} 
  
  void lcdData(int data)
{ digitalWrite(PA14,1);//RS
    wrBus(data /256);
    wrBus(data & 0x00ff);
}

  /*Command($2C, LCD_ExectimeGfx);     // Standby Mode OFF
Command16($0201, LCD_ExectimeGfx); // Oscillation Mode Set
Command16($260F, LCD_ExectimeGfx); // DCDC and AMP ON/OFF set
Command16($1030, LCD_ExectimeGfx); // Driver Output Mode Set
Command16($2001, LCD_ExectimeGfx); // DC-DC Select
Command16($2201, LCD_ExectimeGfx); // Bias Set
Command16($2401, LCD_ExectimeGfx); // DCDC Clock Division Set
Command16($2800, LCD_ExectimeGfx); // Temperature Compensation Set
Command16($3040, LCD_ExectimeGfx); // Addressing Mode Set
Command16($3201, LCD_ExectimeGfx); // ROW Vector Mode Set
Command16($3482, LCD_ExectimeGfx); // N-line Inversion Set
Command16($4000, LCD_ExectimeGfx); // Entry Mode Set


if Contrast_ON then Command16($2A00 + 128 + ContrastValue div 2, LCD_ExectimeGfx) // Contrast
               else Command16($2A00, LCD_ExectimeGfx); // Set contrast OFF
#include "config.h"
#include "cpu.h"
#include "lcd.h"
#include "kernel.h"
#include "system.h"

// Display status 
static unsigned lcd_yuv_options SHAREDBSS_ATTR = 0;

//LCD command set for Samsung S6B33B2 
#define R_NOP                  0x00
#define R_OSCILLATION_MODE     0x02
#define R_DRIVER_OUTPUT_MODE   0x10
#define R_DCDC_SET             0x20
#define R_BIAS_SET             0x22
#define R_DCDC_CLOCK_DIV       0x24
#define R_DCDC_AMP_ONOFF       0x26
#define R_TEMP_COMPENSATION    0x28
#define R_CONTRAST_CONTROL1    0x2a
#define R_CONTRAST_CONTROL2    0x2b
#define R_STANDBY_OFF          0x2c
#define R_STANDBY_ON           0x2d
#define R_DDRAM_BURST_OFF      0x2e
#define R_DDRAM_BURST_ON       0x2f
#define R_ADDRESSING_MODE      0x30
#define R_ROW_VECTOR_MODE      0x32
#define R_N_LINE_INVERSION     0x34
#define R_FRAME_FREQ_CONTROL   0x36
#define R_RED_PALETTE          0x38
#define R_GREEN_PALETTE        0x3a
#define R_BLUE_PALETTE         0x3c
#define R_ENTRY_MODE           0x40
#define R_X_ADDR_AREA          0x42
#define R_Y_ADDR_AREA          0x43
#define R_RAM_SKIP_AREA        0x45
#define R_DISPLAY_OFF          0x50
#define R_DISPLAY_ON           0x51
#define R_SPEC_DISPLAY_PATTERN 0x53
#define R_PARTIAL_DISPLAY_MODE 0x55
#define R_PARTIAL_START_LINE   0x56
#define R_PARTIAL_END_LINE     0x57
#define R_AREA_SCROLL_MODE     0x59
#define R_SCROLL_START_LINE    0x5a
#define R_DATA_FORMAT_SELECT   0x60

// TCC77x specific defines 
#define LCD_BASE  0x50000000
#define LCD_CMD   *(volatile unsigned char*)(LCD_BASE)
#define LCD_DATA  *(volatile unsigned char*)(LCD_BASE+1)

static void lcd_send_command(unsigned cmd)
{
    LCD_CMD = cmd;
        
    asm volatile (
        "nop      \n\t"
        "nop      \n\t"
        "nop      \n\t"
    );
}

static void lcd_send_data(unsigned data)
{
    LCD_DATA = (data & 0xff00) >> 8;
    LCD_DATA = (data & 0x00ff);
}

void lcd_init_device(void)
{
    uint32_t bus_width;

             
             
    GPIOE &= ~0x8;
    sleep(HZ/100);   

    GPIOE |= 0x08;
    sleep(HZ/100);  
    
    lcd_send_command(R_STANDBY_OFF);
    sleep(HZ/20);   

    lcd_send_command(R_OSCILLATION_MODE);
    lcd_send_command(0x01);
    sleep(HZ/100);   

    lcd_send_command(R_DCDC_AMP_ONOFF);
    lcd_send_command(0x01);
    sleep(HZ/100); 

    lcd_send_command(R_DCDC_AMP_ONOFF);
    lcd_send_command(0x09);
    sleep(HZ/100);  

    lcd_send_command(R_DCDC_AMP_ONOFF);
    lcd_send_command(0x0b);
    sleep(HZ/100);  
    lcd_send_command(R_DCDC_AMP_ONOFF);
    lcd_send_command(0x0f);
    sleep(HZ/100); 

    lcd_send_command(R_DCDC_SET);
    lcd_send_command(0x01);
    sleep(HZ/100);     
    sleep(HZ/10);      

    lcd_send_command(R_TEMP_COMPENSATION);
    lcd_send_command(0x01);
    sleep(HZ/100);    

    lcd_send_command(R_DRIVER_OUTPUT_MODE);
    lcd_send_command(0x03);

    lcd_send_command(R_ENTRY_MODE);
    lcd_send_command(0x81);

    lcd_send_command(R_N_LINE_INVERSION);
    lcd_send_command(0x04);
    lcd_send_command(0xfa);
    lcd_send_command(0x5f);

    lcd_set_contrast(0x28);

    lcd_send_command(R_SPEC_DISPLAY_PATTERN);
    lcd_send_command(0x0);
    sleep(HZ/100);   

    lcd_send_command(R_ADDRESSING_MODE);
    lcd_send_command(0x0);
    sleep(HZ/100);   

    lcd_send_command(R_PARTIAL_DISPLAY_MODE);
    lcd_send_command(0x0);
    sleep(HZ/100);    

    lcd_send_command(R_X_ADDR_AREA);
    lcd_send_command(0);
    lcd_send_command(0x80);

    lcd_send_command(R_Y_ADDR_AREA);
    lcd_send_command(0x0);
    lcd_send_command(0x80);

    lcd_send_command(R_DISPLAY_ON);

    lcd_send_command(R_SPEC_DISPLAY_PATTERN);
    lcd_send_command(0x0);
    
    
    lcd_clear_display();
    lcd_update();
}


int lcd_default_contrast(void)
{
    return 0x28;
}

void lcd_set_contrast(int val)
{
    //val &= 0xFF;
    lcd_send_command(R_CONTRAST_CONTROL1);
    lcd_send_command(val);
}

void lcd_set_invert_display(bool yesno)
{
   
    (void)yesno;
}

void lcd_set_flip(bool yesno)
{
    lcd_send_command(R_DRIVER_OUTPUT_MODE);
    lcd_send_command(yesno ? 0x02 : 0x07);
}

void lcd_yuv_set_options(unsigned options)
{
    lcd_yuv_options = options;
}

void lcd_blit_yuv(unsigned char *const src[3],
                  int src_x, int src_y, int stride,
                  int x, int y, int width, int height)
{
    (void) src;
    (void) src_x;
    (void) src_y;
    (void) stride;
    (void) x;
    (void) y;

    return;

}

//Update the display.This must be called after all other LCD functions that change the display. 
void lcd_update(void)
{
    lcd_update_rect(0, 0, LCD_WIDTH, LCD_HEIGHT);
}

// Update a fraction of the display. 
void lcd_update_rect(int x, int y, int width, int height)
{
    const fb_data *addr;
    
    if (x + width >= LCD_WIDTH)
        width = LCD_WIDTH - x;
    if (y + height >= LCD_HEIGHT)
        height = LCD_HEIGHT - y;
        
    if ((width <= 0) || (height <= 0))
        return;

    addr = &lcd_framebuffer[y][x];

    if (width <= 1) {                    
        lcd_send_command(R_ENTRY_MODE);  The X end address must be larger 
        lcd_send_command(0x80);          that the X start address, so we 
        lcd_send_command(R_X_ADDR_AREA); switch to vertical mode for 
        lcd_send_command(x);             single column updates and set 
        lcd_send_command(x + 1);          the window width to 2 
    } else {
        lcd_send_command(R_ENTRY_MODE);
        lcd_send_command(0x82);
        lcd_send_command(R_X_ADDR_AREA);
        lcd_send_command(x);
        lcd_send_command(x + width - 1);
    }

    lcd_send_command(R_Y_ADDR_AREA);
    lcd_send_command(y);
    lcd_send_command(y + height - 1);

    NOP needed because on some c200s, the previous lcd_send_command is
       interpreted as a separate command instead of part of R_Y_ADDR_AREA. 
    lcd_send_command(R_NOP);

    do {
        int w = width;
        do {
            lcd_send_data(*addr++);
        } while (--w > 0);
        addr += LCD_WIDTH - width;
    } while (--height > 0);
}

*/
