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
volatile static uint8_t Flag = 0;

typedef void (*MenuFunctionPtr)(char cmd);

//function pointer
MenuFunctionPtr MFPtr;

//Structure describes current menu and submenu state
struct Menu_State
{
	uint8_t menuNo;//1,2,3,4
	uint8_t subMenuNo;//1,2,3
} MN;


//Functions for each menu item
void MeasureMenu(char cmd);
void MainMenu(char cmd);
void MainSub1(char cmd);
void MainSub2(char cmd);
void MainSub3(char cmd);
void func201(char cmd);
void func202(char cmd);
void func301(char cmd);
void func302(char cmd);
void func401(char cmd);
void func402(char cmd);


//SubMenu and Function table pointer update
uint8_t MFIndex(uint8_t, uint8_t);

void menu_Init(void);
void delay1s(void);

//unsigned char* read_pgm_string(const char *FlashLoc);

#endif /* MENU_H_ */