/*
 * Menu1.h
 *
 * Created: 30.11.2013 15:26:11
 *  Author: john
 */ 


#ifndef MENU1_H_
#define MENU1_H_

#include "global.h"

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

uint8_t startMenu();
void initMenu();

#endif /* MENU1_H_ */