/*
 * ili9341.h
 *
 * Created: 05.11.2013 11:01:46
 *  Author: estarcev
 */ 


#ifndef ILI9341_H_
#define ILI9341_H_

#include "..\..\global.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/pgmspace.h>

#include "..\spi\spi.h"

//Basic Colors
#define RED             0xf800
#define GREEN			0x07e0
#define BLUE			0x001f
#define BLACK			0x0000
#define YELLOW			0xffe0
#define WHITE			0xffff

//Other Colors
#define CYAN            0x07ff
#define BRIGHT_RED      0xf810
#define GRAY1			0x8410
#define GRAY2			0x4208

//TFT resolution 240*320
#define MIN_X			0
#define MIN_Y			0
#define MAX_X			239
#define MAX_Y			319

#define TFT_CS_LOW		{DDRD |= 0x20; PORTD &=~ 0x20;}
#define TFT_CS_HIGH		{DDRD |= 0x20; PORTD |= 0x20;}
#define TFT_DC_LOW		{DDRD |= 0x40; PORTD &=~ 0x40;}
#define TFT_DC_HIGH		{DDRD |= 0x40; PORTD |= 0x40;}
#define TFT_BL_OFF		{DDRD |= 0x80; PORTD &=~ 0x80;}
#define TFT_BL_ON		{DDRD |= 0x80; PORTD |= 0x80;}
#define TFT_RST_OFF		{DDRD |= 0x10; PORTD |= 0x10;}
#define TFT_RST_ON		{DDRD |= 0x10; PORTD &=~ 0x10;}

#define YP				A2 // must be an analog pin, use "An" notation!
#define XM				A1 // must be an analog pin, use "An" notation!
#define YM				14 // can be a digital pin, this is A0
#define XP				17 // can be a digital pin, this is A3

#define TS_MINX			116 * 2
#define TS_MAXX			890 * 2
#define TS_MINY			83 * 2
#define TS_MAXY			913 * 2

#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

extern uint8_t simpleFont[][8];

void TFT_init (void);
void TFT_setCol(uint16_t StartCol, uint16_t EndCol);
void TFT_setPage(uint16_t StartPage, uint16_t EndPage);
void TFT_setXY(uint16_t poX, uint16_t poY);
void TFT_setPixel(uint16_t poX, uint16_t poY, uint16_t color);
void TFT_sendCMD(uint8_t index);
void TFT_WRITE_Package(uint16_t *data, uint8_t howmany);
void TFT_WRITE_DATA(uint8_t data);
void TFT_sendData(uint16_t data);
uint8_t TFT_Read_Register(uint8_t Addr, uint8_t xParameter);
void TFT_fillScreen(uint16_t XL, uint16_t XR, uint16_t YU, uint16_t YD, uint16_t color);
void TFT_fillScreen1(void);
uint8_t TFT_readID(void);

void TFT_drawChar(uint8_t ascii, uint16_t poX, uint16_t poY, uint16_t size, uint16_t fgcolor);
void TFT_drawString(char *string, uint16_t poX, uint16_t poY, uint16_t size, uint16_t fgcolor);
void TFT_fillRectangle(uint16_t poX, uint16_t poY, uint16_t length, uint16_t width, uint16_t color);

void TFT_drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void TFT_drawVerticalLine(uint16_t poX, uint16_t poY,uint16_t length, uint16_t color);
void TFT_drawHorizontalLine(uint16_t poX, uint16_t poY, uint16_t length, uint16_t color);
void TFT_drawRectangle(uint16_t poX, uint16_t poY, uint16_t length, uint16_t width, uint16_t color);

void TFT_drawCircle(int poX, int poY, int r, uint16_t color);
void TFT_fillCircle(int poX, int poY, int r, uint16_t color);

void TFT_drawTriangle(int poX1, int poY1, int poX2, int poY2, int poX3, int poY3, uint16_t color);
uint8_t TFT_drawNumber(long long_num, uint16_t poX, uint16_t poY, uint16_t size, uint16_t fgcolor);
uint8_t TFT_drawFloat(float floatNumber, uint8_t decimal, uint16_t poX, uint16_t poY, uint16_t size, uint16_t fgcolor);
uint8_t TFT_drawFloat1(float floatNumber, uint16_t poX, uint16_t poY, uint16_t size, uint16_t fgcolor);

#endif /* ILI9341_H_ */