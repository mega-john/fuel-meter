/*
 * Display.h
 *
 * Created: 05.01.2014 23:41:31
 *  Author: john
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "ili9341.h"
//#include "ks0108.h"

#define ILI_DISPLAY

void displayInit();
void displayDrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void displaySelectFont(const uint8_t* font, uint8_t color);
void displayClear();
void displayDrawString(const char *string, uint16_t poX, uint16_t poY, uint16_t size, uint16_t fgcolor);
void displayFillRectangle(uint16_t poX, uint16_t poY, uint16_t length, uint16_t width, uint16_t color);

#endif /* DISPLAY_H_ */