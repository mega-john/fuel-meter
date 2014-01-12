/*
 * Display.c
 *
 * Created: 05.01.2014 23:41:20
 *  Author: john
 */ 
#include "Display.h"

void displayInit()
{
#ifdef ILI_DISPLAY
	TFT_init();
#else
	//ks0108Init(0);
#endif	
}

void displayDrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color )
{
#ifdef ILI_DISPLAY
	TFT_drawLine(x0, y0, x1, y1, color);
#else
	//ks0108DrawLine(x0, y0, x1, y1, color);
#endif	
}

void displaySelectFont(const uint8_t* font)
{
#ifdef ILI_DISPLAY
	TFT_SelectFont(font);
#else
	//ks0108SelectFont(font, ks0108ReadFontData, color);
#endif
}

void displayClear()
{
#ifdef ILI_DISPLAY
	TFT_fillScreen(0, 239, 0, 319, TFT_BLACK);
#else
	//ks0108ClearScreen();
#endif
}

void displayDrawString( const char *string, uint16_t poX, uint16_t poY, uint16_t size, uint16_t fgcolor, uint16_t bgcolor)
{
#ifdef ILI_DISPLAY
	TFT_drawString(string, poX, poY, size, fgcolor, bgcolor);
#else
	//ks0108GotoXY(poX, poY);
	//ks0108Puts(string);
#endif	
}

void displayFillRectangle( uint16_t poX, uint16_t poY, uint16_t length, uint16_t width, uint16_t color )
{
#ifdef ILI_DISPLAY
	TFT_fillRectangle(poX, poY, length, width, color);
#else
	//ks0108FillRect(poX, poY, width, length, color);
#endif
}
