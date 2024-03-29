﻿/*
 * Copyright:      Fabian Maximilian Thiele  mailto:me@apetech.de
 * Author:         Fabian Maximilian Thiele
 * Remarks:        this Copyright must be included
 * known Problems: none
 * Version:        1.1
 * Description:    Graphic Library for KS0108- (and compatible) based LCDs
 * 
 */
#include <inttypes.h>
#include <avr/pgmspace.h>
#include "..\..\global.h"
#include "..\74HC595.h"

#ifndef	KS0108_H
#define KS0108_H

// Ports
#define LCD_CMD_PORT		PORTB		// Command Output Register
#define LCD_CMD_DIR			DDRB		// Data Direction Register for Command Port

#define LCD_CSEL_PORT		PORTC
#define LCD_CSEL_DIR		DDRC

//#define LCD_DATA_IN		PINA		// Data Input Register
//#define LCD_DATA_OUT		PORTA		// Data Output Register
//#define LCD_DATA_DIR		DDRA		// Data Direction Register for Data Port

// Command Port Bits
#define D_I					0x03		// D/I Bit Number
#define R_W					0x01		// R/W Bit Number
#define EN					0x00		// EN Bit Number

#define CSEL1				0x07		// CS1 Bit Number
#define CSEL2				0x06		// CS2 Bit Number

// Chips
#define CHIP1				0x00
#define CHIP2				0x01

// Commands
#define LCD_ON				0x3F
#define LCD_OFF				0x3E
#define LCD_SET_ADD			0x40
#define LCD_SET_PAGE		0xB8
#define LCD_DISP_START		0xC0

// Colors
#define BLACK				0xFF
#define WHITE				0x00

// Font Indices
#define FONT_LENGTH			0
#define FONT_FIXED_WIDTH	2
#define FONT_HEIGHT			3
#define FONT_FIRST_CHAR		4
#define FONT_CHAR_COUNT		5
#define FONT_WIDTH_TABLE	6

#define SCREEN_WIDTH		128
#define SCREEN_HEIGHT		64

// Uncomment for slow drawing
// #define DEBUG

typedef struct {
	uint8_t x;
	uint8_t y;
	uint8_t page;
} lcdCoord;

typedef uint8_t (*ks0108FontCallback)(const uint8_t*);

//
// Function Prototypes
//

// Graphic Functions
void ks0108DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);
void ks0108DrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);
void ks0108DrawRoundRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t radius, uint8_t color);
void ks0108FillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);
void ks0108InvertRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
void ks0108SetInverted(uint8_t invert);
void ks0108SetDot(uint8_t x, uint8_t y, uint8_t color);

#define ks0108DrawVertLine(x, y, length, color) {ks0108FillRect(x, y, 0, length, color);}
#define ks0108DrawHoriLine(x, y, length, color) {ks0108FillRect(x, y, length, 0, color);}
#define ks0108DrawCircle(xCenter, yCenter, radius, color) {ks0108DrawRoundRect(xCenter-radius, yCenter-radius, 2*radius, 2*radius, radius, color);}
#define ks0108ClearScreen() {ks0108FillRect(0, 0, 127, 63, WHITE);}
#define isFixedWidthFont(font)  ((ks0108FontRead(font + FONT_LENGTH) == 0) && (ks0108FontRead(font + FONT_LENGTH + 1) == 0)))

// Font Functions
uint8_t ks0108ReadFontData(const uint8_t* ptr);		//Standard Read Callback
void ks0108SelectFont(const uint8_t* font, ks0108FontCallback callback, uint8_t color);
int ks0108PutChar(char c);
void ks0108Puts(char* str);
void ks0108Puts_P(PGM_P str);
uint8_t ks0108CharWidth(char c);
uint8_t ks0108StringWidth(char* str);
uint8_t ks0108StringWidth_P(PGM_P str);

// Control Functions
void ks0108GotoXY(uint8_t x, uint8_t y);
void ks0108Init(uint8_t invert);
//inline uint8_t ks0108ReadData(void);
void ks0108WriteCommand(uint8_t cmd, uint8_t chip);
void ks0108WriteData(uint8_t data);

void ks0108Enable(void);
uint8_t ks0108ReadData(void);
#endif
