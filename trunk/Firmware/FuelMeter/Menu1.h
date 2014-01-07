/*
 * Menu1.h
 *
 * Created: 30.11.2013 15:26:11
 *  Author: john
 */ 


#ifndef MENU1_H_
#define MENU1_H_

#include <avr/pgmspace.h>
#include <stdbool.h>
#include "global.h"
#include "hardware/display/display.h"

enum
{
	MENU_CANCEL = 1,
	MENU_RESET,
	MENU_MODE1,
	MENU_MODE2,
	MENU_MODE3,
	MENU_SENS1,
	MENU_SENS2,
	MENU_SENS3,
	MENU_WARM,
	MENU_PROCESS
};

void InitMenu();
void ProcessMenu(uint8_t cmd);

#endif /* MENU1_H_ */