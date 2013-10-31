/*
 * Copyright:      Fabian Maximilian Thiele  mailto:me@apetech.de
 * Author:         Fabian Maximilian Thiele
 * Remarks:        this Copyright must be included
 * known Problems: none
 * Version:        1.1
 * Description:    Graphic Library for KS0108- (and compatible) based LCDs
 * 
 */

#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "ks0108.h"

lcdCoord			ks0108Coord;
uint8_t				ks0108Inverted = 0x00;
ks0108FontCallback	ks0108FontRead = 0x00;
uint8_t				ks0108FontColor = 0x00;
const uint8_t*		ks0108Font = 0x00;
uint8_t				displayBuffer[1024];

//inline uint8_t ks0108ReadData(void);

void ks0108DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color) 
{
	uint8_t length, i, y, yAlt, xTmp, yTmp;
	int16_t m;
	
	//
	// vertical line
	//
	if(x1 == x2) 
	{
		// x1|y1 must be the upper point
		if(y1 > y2)
		{
			yTmp = y1;
			y1 = y2;
			y2 = yTmp;
		}
		ks0108DrawVertLine(x1, y1, y2 - y1, color);
	
	//
	// horizontal line
	//
	} 
	else if(y1 == y2) 
	{	
		// x1|y1 must be the left point
		if(x1 > x2) 
		{
			xTmp = x1;
			x1 = x2;
			x2 = xTmp;
		}
		ks0108DrawHoriLine(x1, y1, x2 - x1, color);
	
	//
	// schiefe line :)
	//
	} 
	else 
	{
		// angle >= 45°
		if((y2 - y1) >= (x2 - x1) || (y1 - y2) >= (x2 - x1)) 
		{
			// x1 must be smaller than x2
			if(x1 > x2) 
			{
				xTmp = x1;
				yTmp = y1;
				x1 = x2;
				y1 = y2;
				x2 = xTmp;
				y2 = yTmp;
			}
		
			length = x2 - x1;		// not really the length :)
			m = ((y2 - y1) * 200) / length;
			yAlt = y1;
			
			for(i = 0; i <= length; i++) 
			{
				y = ((m * i) / 200) + y1;
				
				if((m * i) % 200 >= 100)
				{
					y++;
				}
				else if((m * i) % 200 <= -100)
				{
					y--;
				}
				
				ks0108DrawLine(x1 + i, yAlt, x1 + i, y, color);
				
				if(length <= (y2 - y1) && y1 < y2)
				{
					yAlt = y + 1;
				}
				else if(length <= (y1 - y2) && y1 > y2)
				{
					yAlt = y - 1;
				}
				else
				{
					yAlt = y;
				}
			}
		
		// angle < 45°
		} 
		else 
		{
			// y1 must be smaller than y2
			if(y1 > y2) 
			{
				xTmp = x1;
				yTmp = y1;
				x1 = x2;
				y1 = y2;
				x2 = xTmp;
				y2 = yTmp;
			}
			
			length = y2 - y1;
			m = ((x2 - x1) * 200) / length;
			yAlt = x1;
			
			for(i = 0; i <= length; i++) 
			{
				y = ((m * i) / 200) + x1;
				
				if((m * i) % 200 >= 100)
				{
					y++;
				}
				else if((m * i) % 200 <= -100)
				{
					y--;
				}
				
				ks0108DrawLine(yAlt, y1 + i, y, y1 + i, color);
				if(length <= (x2 - x1) && x1 < x2)
				{
					yAlt = y + 1;
				}
				else if(length <= (x1 - x2) && x1 > x2)
				{
					yAlt = y - 1;
				}
				else
				{
					yAlt = y;
				}
			}
		}
	}
}

void ks0108DrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color) 
{
	ks0108DrawHoriLine(x, y, width, color);				// top
	ks0108DrawHoriLine(x, y + height, width, color);	// bottom
	ks0108DrawVertLine(x, y, height, color);			// left
	ks0108DrawVertLine(x + width, y, height, color);	// right
}

void ks0108DrawRoundRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t radius, uint8_t color)
{
	uint8_t x1 = 0;
	uint8_t y1 = radius;
	int16_t tSwitch = 3 - 2 * radius;
	
	while (x1 <= y1)
	{
		ks0108SetDot(x + radius - x1, y + radius - y1, color);
		ks0108SetDot(x + radius - y1, y + radius - x1, color);

		ks0108SetDot(x + width - radius + x1, y + radius - y1, color);
		ks0108SetDot(x + width - radius + y1, y + radius - x1, color);
		
		ks0108SetDot(x + width - radius + x1, y + height - radius + y1, color);
		ks0108SetDot(x + width - radius + y1, y + height - radius + x1, color);

		ks0108SetDot(x + radius - x1, y + height - radius + y1, color);
		ks0108SetDot(x + radius - y1, y + height - radius + x1, color);

		if (tSwitch < 0)
		{
			tSwitch += (4 * x1 + 6);
		}
		else
		{
			tSwitch += (4 * (x1 - y1) + 10);
			y1--;
		}
		x1++;
	}
	
	ks0108DrawHoriLine(x + radius, y, width-(2 * radius), color);			// top
	ks0108DrawHoriLine(x + radius, y + height, width - (2 * radius), color);// bottom
	ks0108DrawVertLine(x, y + radius, height - (2 * radius), color);		// left
	ks0108DrawVertLine(x + width, y + radius, height - (2 * radius), color);// right
}
/*
 * Hardware-Functions 
 */
void ks0108FillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color) 
{
	uint8_t mask = 0x00;
	uint8_t pageOffset = 0x00;
	uint8_t h = 0x00;
	uint8_t i = 0x00;
	uint8_t data = 0x00;
	height++;
	
	pageOffset = y % 8;
	y -= pageOffset;
	mask = 0xFF;
	if(height < 8 - pageOffset)
	{
		mask >>= (8 - height);
		h = height;
	} 
	else 
	{
		h = 8 - pageOffset;
	}
	mask <<= pageOffset;
	
	ks0108GotoXY(x, y);
	for(i = 0; i <= width; i++) 
	{
		//data = ks0108ReadData();
		
		if(color == BLACK) 
		{
			data |= mask;
		} 
		else 
		{
			data &= ~mask;
		}

		ks0108WriteData(data);
	}
	
	while(h + 8 <= height) 
	{
		h += 8;
		y += 8;
		ks0108GotoXY(x, y);
		
		for(i = 0; i <= width; i++) 
		{
			ks0108WriteData(color);
		}
	}
	
	if(h < height) 
	{
		mask = ~(0xFF << (height - h));
		ks0108GotoXY(x, y + 8);
		
		for(i = 0; i <= width; i++) 
		{
			//data = ks0108ReadData();
		
			if(color == BLACK) 
			{
				data |= mask;
			} 
			else 
			{
				data &= ~mask;
			}
	
			ks0108WriteData(data);
		}
	}
}

void ks0108InvertRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height) 
{
	uint8_t mask = 0x00;
	uint8_t pageOffset = 0x00;
	uint8_t h = 0x00;
	uint8_t i = 0x00;
	uint8_t data = 0x00;
	//uint8_t tmpData = 0x00;
	height++;
	
	pageOffset = y % 8;
	y -= pageOffset;
	mask = 0xFF;
	if(height < 8 - pageOffset) 
	{
		mask >>= (8 - height);
		h = height;
	} 
	else 
	{
		h = 8 - pageOffset;
	}
	mask <<= pageOffset;
	
	ks0108GotoXY(x, y);
	for(i = 0; i <= width; i++) 
	{
		//data = ks0108ReadData();
		//tmpData = ~data;
		//data = (tmpData & mask) | (data & ~mask);
		ks0108WriteData(data);
	}
	
	while(h + 8 <= height) 
	{
		h += 8;
		y += 8;
		ks0108GotoXY(x, y);
		
		for(i = 0; i <= width; i++) 
		{
			//data = ks0108ReadData();
			ks0108WriteData(~data);
		}
	}
	
	if(h < height) 
	{
		mask = ~(0xFF << (height - h));
		ks0108GotoXY(x, y + 8);
		
		for(i = 0; i <= width; i++) 
		{
			//data = ks0108ReadData();
			//tmpData = ~data;
			//data = (tmpData & mask) | (data & ~mask);
			ks0108WriteData(data);
		}
	}
}

void ks0108SetInverted(uint8_t invert) 
{
	if(ks0108Inverted != invert) 
	{
		ks0108InvertRect(0, 0, 127, 63);
		ks0108Inverted = invert;
	}
}

void ks0108SetDot(uint8_t x, uint8_t y, uint8_t color) 
{
	uint8_t data = 0x00;
	
	ks0108GotoXY(x, y - y % 8);					// read data from display memory
	//data = ks0108ReadData();
	
	if(color == BLACK) 
	{
		data |= 0x01 << (y % 8);				// set dot
	} 
	else 
	{
		data &= ~(0x01 << (y % 8));			// clear dot
	}
	
	ks0108WriteData(data);					// write data back to display
}

uint8_t ks0108ReadFontData(const uint8_t* ptr) 
{
	return pgm_read_byte(ptr);
}

void ks0108SelectFont(const uint8_t* font, ks0108FontCallback callback, uint8_t color) 
{
	ks0108Font = font;
	ks0108FontRead = callback;
	ks0108FontColor = color;
}

int ks0108PutChar(char c) 
{
	uint8_t width = 0;
	uint8_t height = ks0108FontRead(ks0108Font + FONT_HEIGHT);
	uint8_t bytes = (height + 7) >> 3;// / 8;
	
	uint8_t firstChar = ks0108FontRead(ks0108Font + FONT_FIRST_CHAR);
	uint8_t charCount = ks0108FontRead(ks0108Font + FONT_CHAR_COUNT);
	
	uint16_t index = 0;
	uint8_t x = ks0108Coord.x;
	uint8_t y = ks0108Coord.y;
	//uint8_t thielefont;

	
	if(c < firstChar || c >= (firstChar + charCount)) 
	{
		return 1;
	}
	else if(c > 127)
	{
		c -= 0x40;
	}
	//else
	{
		c -= firstChar;
	}
	
	if(isFixedWidthFont(ks0108Font) 
	{
		//thielefont = 0;
		width = ks0108FontRead(ks0108Font + FONT_FIXED_WIDTH);
		index = c * bytes * width + FONT_WIDTH_TABLE;
	}
	else
	{
		//thielefont = 1;
		for(uint8_t i = 0; i < c; i++) 
		{  
			index += ks0108FontRead(ks0108Font + FONT_WIDTH_TABLE + i);
		}
	   index = index * bytes + charCount + FONT_WIDTH_TABLE;
	   width = ks0108FontRead(ks0108Font + FONT_WIDTH_TABLE + c);	
	}
	
	// read width data, to get the index
	//for(uint8_t i = 0; i < c; i++) 
	//{
		//index += ks0108FontRead(ks0108Font + FONT_WIDTH_TABLE + i);
	//}
	//index = index * bytes + charCount + FONT_WIDTH_TABLE;
	//width = ks0108FontRead(ks0108Font + FONT_WIDTH_TABLE + c);
	
	// last but not least, draw the character
	for(uint8_t i = 0; i < bytes; i++) 
	{
		uint8_t page = i * width;
		for(uint8_t j = 0; j < width; j++) 
		{
			uint8_t data = ks0108FontRead(ks0108Font + index + page + j);
			
			if((height > 8) && (height < (i + 1) << 3)) 
			{
				data >>= (((i + 1) << 3) - height);
			}
			
			if(ks0108FontColor == BLACK) 
			{
				ks0108WriteData(data);
			} 
			else 
			{
				ks0108WriteData(~data);
			}
		}
		// 1px gap between chars
		if(ks0108FontColor == BLACK) 
		{
			ks0108WriteData(0x00);
		} 
		else 
		{
			ks0108WriteData(0xFF);
		}
		ks0108GotoXY(x, ks0108Coord.y + 8);
	}
	ks0108GotoXY(x + width + 1, y);
	
	return 0;
}

void ks0108Puts(char* str) 
{
	int x = ks0108Coord.x;
	while(*str != 0) 
	{
		if(*str == '\n') 
		{
			ks0108GotoXY(x, ks0108Coord.y + ks0108FontRead(ks0108Font + FONT_HEIGHT));
		} 
		else 
		{
			ks0108PutChar(*str);
		}
		str++;
	}
}

void ks0108Puts_P(PGM_P str) 
{
	int x = ks0108Coord.x;
	while(pgm_read_byte(str) != 0) 
	{
		if(pgm_read_byte(str) == '\n') 
		{
			ks0108GotoXY(x, ks0108Coord.y + ks0108FontRead(ks0108Font + FONT_HEIGHT));
		} 
		else 
		{
			ks0108PutChar(pgm_read_byte(str));
		}
		str++;
	}
}

uint8_t ks0108CharWidth(char c) 
{
	uint8_t width = 0;
	uint8_t firstChar = ks0108FontRead(ks0108Font + FONT_FIRST_CHAR);
	uint8_t charCount = ks0108FontRead(ks0108Font + FONT_CHAR_COUNT);
	
	// read width data
	if(c >= firstChar && c < (firstChar + charCount)) 
	{
		c -= firstChar;
		width = ks0108FontRead(ks0108Font + FONT_WIDTH_TABLE + c) + 1;
	}
	
	return width;
}

uint8_t ks0108StringWidth(char* str) 
{
	uint8_t width = 0;
	
	while(*str != 0) 
	{
		width += ks0108CharWidth(*str++);
	}
	
	return width;
}

uint8_t ks0108StringWidth_P(PGM_P str) 
{
	uint8_t width = 0;
	
	while(pgm_read_byte(str) != 0) 
	{
		width += ks0108CharWidth(pgm_read_byte(str++));
	}
	
	return width;
}

void ks0108GotoXY(uint8_t x, uint8_t y) 
{
	uint8_t chip = CHIP1, cmd;
	
	if(x > 127) 
	{
		x = 0;								// ensure that coordinates are legal
	}
	if(y > 63)  
	{
		y = 0;
	}
	
	ks0108Coord.x = x;								// save new coordinates
	ks0108Coord.y = y;
	ks0108Coord.page = y >> 3;
	
	if(x >= 64) 
	{									// select the right chip
		x -= 64;
		chip = CHIP2;
	}
	cmd = LCD_SET_ADD | x;
	ks0108WriteCommand(cmd, chip);					// set x address on active chip
	
	cmd = LCD_SET_PAGE | ks0108Coord.page;			// set y address on both chips
	ks0108WriteCommand(cmd, CHIP1);
	ks0108WriteCommand(cmd, CHIP2);
}

inline void ks0108InitPorts()
{
	//LCD_DATA_DIR	 = 0xFF;
	//LCD_DATA_OUT 	 = 0;
	LCD_CMD_DIR 	|= (1<<D_I)|(1<<R_W)|(1<<EN);
	LCD_CMD_PORT 	|= (0<<EN);
	LCD_CSEL_PORT   |= (1<<CSEL1)|(1<<CSEL2);
	LCD_CSEL_DIR	|= (1<<CSEL1)|(1<<CSEL2);
}

void ks0108Init(uint8_t invert)
{
	ks0108InitPorts();
	ks0108Coord.x = 0;
	ks0108Coord.y = 0;
	ks0108Coord.page = 0;
	
	ks0108Inverted = invert;
	
	LCD_CMD_DIR = 0xFF;								// command port is output
	LCD_CSEL_DIR = 0xFF;
	ks0108WriteCommand(LCD_ON, CHIP1);				// power on
	ks0108WriteCommand(LCD_ON, CHIP2);
	
	ks0108WriteCommand(LCD_SET_ADD, CHIP1);				// power on
	ks0108WriteCommand(LCD_SET_ADD, CHIP2);
	
	ks0108WriteCommand(LCD_SET_PAGE, CHIP1);				// power on
	ks0108WriteCommand(LCD_SET_PAGE, CHIP2);
	
	ks0108WriteCommand(LCD_DISP_START, CHIP1);		// display start line = 0
	ks0108WriteCommand(LCD_DISP_START, CHIP2);
	ks0108ClearScreen();							// display clear
	//ks0108GotoXY(0, 0);
}

inline void ks0108Enable(void) 
{
	sb(LCD_CMD_PORT, EN);// EN high level width: min. 450ns
	_delay_us(10);

	//asm volatile("nop\n\t"
	//"nop\n\t"
	//"nop\n\t"
	//"nop\n\t"
	//"nop\n\t"
	//"nop\n\t"
	//"nop\n\t"
	//"nop\n\t"
	//"nop\n\t"
	//"nop\n\t"
	//::);
	
	cb(LCD_CMD_PORT, EN);
	for(volatile uint8_t i = 0; i < 8; i++);// a little delay loop (faster than reading the busy flag)
}

//uint8_t ks0108DoReadData(uint8_t first) 
//{
	//uint8_t data;
	//volatile uint8_t i;
	//
	////LCD_DATA_OUT = 0x00;
	////LCD_DATA_DIR = 0x00;// data port is input
	//
	//if(ks0108Coord.x < 64) 
	//{
		//cb(LCD_CSEL_PORT, CSEL2);// deselect chip 2
		//sb(LCD_CSEL_PORT, CSEL1);// select chip 1
	//} 
	//else if(ks0108Coord.x >= 64) 
	//{
		//cb(LCD_CSEL_PORT, CSEL1);// deselect chip 2
		//sb(LCD_CSEL_PORT, CSEL2);// select chip 1
	//}
	//if(ks0108Coord.x == 64 && first) 
	//{	// chip2 X-address = 0
		//ks0108WriteCommand(LCD_SET_ADD, CHIP2);// wuff wuff
	//}
	//
	//sb(LCD_CMD_PORT, D_I);// D/I = 1
	//sb(LCD_CMD_PORT, R_W);// R/W = 1	
	//sb(LCD_CMD_PORT, EN); // EN high level width: min. 450ns
		//
	//_delay_us(10);
	////asm volatile("nop\n\t"
				 ////"nop\n\t"
				 ////"nop\n\t"
				 ////"nop\n\t"
				 ////"nop\n\t"
				 ////"nop\n\t"
				 ////"nop\n\t"
				 ////"nop\n\t"
				 ////"nop\n\t"
				 ////"nop\n\t"
				 ////::);
	//
	//data = 0x00;//LCD_DATA_IN;// read Data			 
	//
	//cb(LCD_CMD_PORT, EN);
	//for(i = 0; i < 8; i++);// a little delay loop (faster than reading the busy flag)
	//
	////LCD_DATA_DIR = 0xFF;
	//
	//ks0108GotoXY(ks0108Coord.x, ks0108Coord.y);
	//
	//if(ks0108Inverted)
	//{
		//data = ~data;
	//}
	//return data;
//}
//
//inline uint8_t ks0108ReadData(void)
//{
	//ks0108DoReadData(1);// dummy read
	//return ks0108DoReadData(0);// "real" read
//}
//
void ks0108WriteCommand(uint8_t cmd, uint8_t chip) 
{
	if(chip == CHIP1) 
	{
		cb(LCD_CSEL_PORT, CSEL2);// deselect chip 2
		sb(LCD_CSEL_PORT, CSEL1);// select chip 1
	} 
	else if(chip == CHIP2) 
	{
		cb(LCD_CSEL_PORT, CSEL1);// deselect chip 1
		sb(LCD_CSEL_PORT, CSEL2);// select chip 2
	}
	
	cb(LCD_CMD_PORT, D_I);// D/I = 0
	cb(LCD_CMD_PORT, R_W);// R/W = 0
	//LCD_DATA_DIR = 0xFF;							// data port is output
	Shift74HC595_Write8(cmd);
	//LCD_DATA_OUT = cmd;								// write command
	ks0108Enable();									// enable
	//LCD_DATA_OUT = 0x00;
}

void ks0108WriteData(uint8_t data) 
{
	uint8_t displayData = 0x00;
	uint8_t yOffset = 0x00;
	uint8_t cmdPort = 0x00;
	uint8_t cselPort = 0x00;

//#ifdef DEBUG
	//volatile uint16_t i;
	//for(i = 0; i < 5000; i++);
//#endif

	if(ks0108Coord.x >= 128)
	{
		return;
	}

	if(ks0108Coord.x < 64) 
	{
		cb(LCD_CSEL_PORT, CSEL2);// deselect chip 2
		sb(LCD_CSEL_PORT, CSEL1);// select chip 1
	} 
	else if(ks0108Coord.x >= 64) 
	{
		cb(LCD_CSEL_PORT, CSEL1);// deselect chip 1
		sb(LCD_CSEL_PORT, CSEL2);// select chip 2
	}
	if(ks0108Coord.x == 64)// chip2 X-address = 0
	{
		ks0108WriteCommand(LCD_SET_ADD, CHIP2);
	}
	
	sb(LCD_CMD_PORT, D_I);// D/I = 1
	cb(LCD_CMD_PORT, R_W);// R/W = 0

	//LCD_DATA_DIR = 0xFF;// data port is output
		
	yOffset = ks0108Coord.y % 8;
	if(yOffset != 0) 
	{
		// first page
		//cmdPort = LCD_CMD_PORT;						// save command port
		//cselPort = LCD_CSEL_PORT;
		//displayData = ks0108ReadData();
		
		//LCD_CMD_PORT = cmdPort;						// restore command port
		//LCD_CSEL_PORT = cselPort;
		//LCD_DATA_DIR = 0xFF;						// data port is output
		
		displayData |= data << yOffset;
		if(ks0108Inverted)
		{
			displayData = ~displayData;
		}
		Shift74HC595_Write8(displayData);
		//LCD_DATA_OUT = displayData;					// write data
		ks0108Enable();								// enable
		
		// second page
		ks0108GotoXY(ks0108Coord.x, ks0108Coord.y + 8);
		
		//displayData = ks0108ReadData();
		
		//LCD_CMD_PORT = cmdPort;						// restore command port
		//LCD_CSEL_PORT = cselPort;
		//LCD_DATA_DIR = 0xFF;						// data port is output
		
		displayData |= data >> (8 - yOffset);
		if(ks0108Inverted)
		{
			displayData = ~displayData;
		}
		Shift74HC595_Write8(displayData);
		//LCD_DATA_OUT = displayData;					// write data
		ks0108Enable();								// enable
		
		ks0108GotoXY(ks0108Coord.x + 1, ks0108Coord.y - 8);
	} 
	else 
	{
		if(ks0108Inverted)
		{
			data = ~data;
		}
		Shift74HC595_Write8(data);
		//LCD_DATA_OUT = data;						// write data
		ks0108Enable();								// enable
		ks0108Coord.x++;
	}
	//LCD_DATA_OUT = 0x00;
}
