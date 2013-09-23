/*
 * Menu.h
 *
 * Created: 02.09.2013 10:40:59
 *  Author: estarcev
 */ 

#ifndef MENU_H_
#define MENU_H_
#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdlib.h>                                                                                                                                     
#include <avr/pgmspace.h>
#include "ks0108.h"
#include "arial_bold_14.h"
#include "corsiva_12.h"
#include "sc.h"

//flag required for exiting from functions loops
static volatile uint8_t Flag = 0;

typedef void (*MenuFunctionPtr)(uint8_t cmd);

//function pointer
MenuFunctionPtr MFPtr;

//Structure describes current menu and submenu state
typedef struct 
{
	uint8_t menuNo;//1,2,3,4
	uint8_t subMenuNo;//1,2,3
} Menu_State;

//Functions for each menu item
void MeasurePage(uint8_t cmd);
void HistoryPageMenu(uint8_t cmd);
void ViewHistory(uint8_t cmd);
void HistoryPage(uint8_t cmd);
void AverageHistory(uint8_t cmd);
void AveragePage(uint8_t cmd);
void ResetHistory(uint8_t cmd);

void menu_init(void);

#endif /* MENU_H_ */