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
#include "font12x16.h"

//flag required for exiting from functions loops
static volatile uint8_t Flag = 0;

typedef void (*MenuFunctionPtr)(uint8_t cmd);

//function pointer
MenuFunctionPtr MFPtr;

//Structure describes current menu and submenu state
struct Menu_State
{
	uint8_t menuNo;//1,2,3,4
	uint8_t subMenuNo;//1,2,3
} MN;


//Functions for each menu item
void MeasureMenu(uint8_t cmd);
void MainMenu(uint8_t cmd);
void MainSub1(uint8_t cmd);
void MainSub2(uint8_t cmd);
void MainSub3(uint8_t cmd);
void func201(uint8_t cmd);
void func202(uint8_t cmd);
void func301(uint8_t cmd);
void func302(uint8_t cmd);
void func401(uint8_t cmd);
void func402(uint8_t cmd);


//SubMenu and Function table pointer update
uint8_t MFIndex(uint8_t, uint8_t);

void menu_init(void);
void delay1s(void);
void set_menu(uint8_t menu_index);

//uint8_t* read_pgm_string(const char *FlashLoc);

#endif /* MENU_H_ */