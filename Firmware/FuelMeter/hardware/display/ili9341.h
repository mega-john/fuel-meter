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
#include "..\display\fonts\fonts.h"

//Basic Colors
#define TFT_RED             0xf800
#define TFT_GREEN			0x07e0
#define TFT_BLUE			0x001f
#define TFT_BLACK			0x0000
#define TFT_YELLOW			0xffe0
#define TFT_WHITE			0xffff

//Other Colors
#define TFT_CYAN			0x07ff
#define TFT_PINK			0xf810
#define TFT_GRAY1			0x8410
#define TFT_GRAY2			0x4208
#define TFT_DARK_GREEN		0x02e0
#define TFT_MAGENTA			0xf8df

//TFT resolution 240*320
#define MIN_X			0U
#define MIN_Y			0U
#define MAX_X			239U
#define MAX_Y			319U

#define TFT_DDR			DDRC
#define TFT_PORT		PORTC

#define TFT_RST			(1 << PINC0)
#define TFT_RST_OFF		{TFT_PORT |= TFT_RST;}		
#define TFT_RST_ON		{TFT_PORT &=~ TFT_RST;}	
	
#define TFT_CS			(1 << PINC1)
#define TFT_CS_LOW		{TFT_PORT &=~ TFT_CS;}		
#define TFT_CS_HIGH		{TFT_PORT |= TFT_CS;}	
		
#define TFT_DC			(1 << PINC2)
#define TFT_DC_LOW		{TFT_PORT &=~ TFT_DC;}		
#define TFT_DC_HIGH		{TFT_PORT |= TFT_DC;}		
	
//background light, at now not used
//#define TFT_BL			(1 << PINC3)//0x80
//#define TFT_BL_OFF		{TFT_DDR |= TFT_BL; TFT_PORT &=~ TFT_BL;}		
//#define TFT_BL_ON			{TFT_DDR |= TFT_BL; TFT_PORT |= TFT_BL;}		

#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

//extern const uint8_t simpleFont[][5];
//extern uint8_t const simpleFont[][8];

typedef uint8_t (*FontCallback)(const uint8_t*);


/**Macro definitions for char display direction**/
#define LEFT2RIGHT 0
#define DOWN2UP    1
#define RIGHT2LEFT 2
#define UP2DOWN    3

void TFT_init (void);
void TFT_setCol(uint16_t StartCol, uint16_t EndCol);
void TFT_setPage(uint16_t StartPage, uint16_t EndPage);
void TFT_setXY(uint16_t poX, uint16_t poY);
void TFT_setPixel(uint16_t poX, uint16_t poY, uint16_t color);
void TFT_sendCMD(uint8_t index);
void TFT_WRITE_Package(uint16_t *data, uint8_t howmany);
void TFT_WRITE_DATA(uint8_t data);
void TFT_sendData(uint16_t data);
//uint8_t TFT_Read_Register(uint8_t Addr, uint8_t xParameter);
void TFT_fillScreen(uint16_t XL, uint16_t XR, uint16_t YU, uint16_t YD, uint16_t color);
void TFT_fillScreen1(void);
//uint8_t TFT_readID(void);

uint8_t TFT_drawChar(uint8_t ascii, uint16_t poX, uint16_t poY, uint16_t size, uint16_t fgcolor);
void TFT_drawString(const char *string, uint16_t poX, uint16_t poY, uint16_t size, uint16_t fgcolor);
void TFT_fillRectangle(uint16_t poX, uint16_t poY, uint16_t length, uint16_t width, uint16_t color);

void TFT_drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void TFT_drawVerticalLine(uint16_t poX, uint16_t poY,uint16_t length, uint16_t color);
void TFT_drawHorizontalLine(uint16_t poX, uint16_t poY, uint16_t length, uint16_t color);
void TFT_drawRectangle(uint16_t poX, uint16_t poY, uint16_t length, uint16_t width, uint16_t color);

void TFT_drawCircle(int poX, int poY, int r, uint16_t color);
void TFT_fillCircle(int poX, int poY, int r, uint16_t color);

void TFT_drawTriangle(int poX1, int poY1, int poX2, int poY2, int poX3, int poY3, uint16_t color);
uint8_t TFT_drawNumber(long long_num, uint16_t poX, uint16_t poY, uint16_t size, uint16_t fgcolor, uint16_t* out_width);
uint8_t TFT_drawFloat(float floatNumber, uint8_t decimal, uint16_t poX, uint16_t poY, uint16_t size, uint16_t fgcolor);
uint8_t TFT_drawFloatRound(float floatNumber, uint16_t poX, uint16_t poY, uint16_t size, uint16_t fgcolor);

//void TFT_setOrientation(uint8_t HV);
void TFT_setDisplayDirect(uint8_t data);
void TFT_set_orientation(uint8_t orientation);


// Font Functions
#define TFT_isFixedWidthFont(font)  ((FontRead(font + FONT_LENGTH) == 0) && (FontRead(font + FONT_LENGTH + 1) == 0))
void TFT_SelectFont(const uint8_t* font, FontCallback callback, uint8_t color);
uint8_t TFT_ReadFontData(const uint8_t* ptr);		//Standard Read Callback
uint16_t TFT_StringWidth(const char* str);

// Font Indices
#define FONT_LENGTH			0
#define FONT_FIXED_WIDTH	2
#define FONT_HEIGHT			3
#define FONT_FIRST_CHAR		4
#define FONT_CHAR_COUNT		5
#define FONT_WIDTH_TABLE	6

#endif /* ILI9341_H_ */