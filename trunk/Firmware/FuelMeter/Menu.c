/*
 * Menu.c
 *
 * Created: 31.08.2013 21:30:19
 *  Author: john
 */ 
#include <stdio.h>

#include "global.h"
#include "Menu.h"

#define MAX_MENU_LENGTH 32

const char main_menu[][MAX_MENU_LENGTH] PROGMEM =
{
	{"1 measure\0"},
	{"2 history \0"},
	{"3 calibrate\0"}
};

const char sub_menu_0[][MAX_MENU_LENGTH] PROGMEM =
{
	{"11 real time\0"},
};

const char sub_menu_1[][MAX_MENU_LENGTH] PROGMEM =
{
	{"21 view\0"},
	{"22 average\0"},
	{"23 reset\0"}
};

const char sub_menu_2[][MAX_MENU_LENGTH] PROGMEM =
{
	{"31 default\0"},
	{"32 start\0"},
};

//static volatile uint8_t menu_index = 0;
extern status_flags flags;


//Menu structure
const uint8_t menu_structure[] PROGMEM =
{
	0,
	3,	//number of main menu items
	1,	//Number of submenu items of menu item 1
	3,	//of menu item 2
	2,	//of menu item 3
};

//Arrray of function pointers in Flash
const MenuFunctionPtr FuncPtrTable[] PROGMEM=
{
	MeasureMenu,
	MainMenu,
	MainSub1, MainSub2, MainSub3,	//functions for submenus of menu 1
	func201, func202, 			//functions for submenus of menu 2
	func301, func302, 			//functions for submenus of menu 3
	func401, func402			//functions for submenus of menu 4
};

const uint8_t MenuIndexes[] PROGMEM = 
{
	0, 1, 2, 3,
	10, 11, 12 ,13,
	20, 21, 22, 23,	
};

volatile uint8_t menu_stack_pointer;
volatile uint8_t menu_stack[10];


void menu_init(void)
{
	menu_stack_pointer = 0;
	MN.menuNo = 0;
	MN.subMenuNo = 0;
	MFPtr = (MenuFunctionPtr)pgm_read_word(&FuncPtrTable[0]);
}

uint8_t MFIndex(uint8_t mn, uint8_t sb)
{
	uint8_t p = 0;//points to menu in table of function pointer
	for(uint8_t i = 0; i < (mn - 1); i++)
	{
		p = p + pgm_read_byte(&menu_structure[i + 1]);
	}
	p = p + sb - 1;
	return p;
}

void delay1s(void)
{
	uint8_t i;
	for(i = 0; i < 10; i++)
	{
		_delay_ms(10);
	}
}

void MainMenu( uint8_t cmd )
{
	if(flags.update_menu == 1)
	{
		//ks0108ClearScreen();
		for (uint8_t i = 0; i < pgm_read_byte(&menu_structure[MN.menuNo]); i++)
		{
			ks0108GotoXY(2, i * 15);
			ks0108Puts_P(main_menu[i]);
			if(MN.subMenuNo == i)
			{
				ks0108DrawRect(0, i * 15, ks0108StringWidth_P(main_menu[i]) + 3, 14, BLACK);
			}
			else
			{
				ks0108DrawRect(0, i * 15, ks0108StringWidth_P(main_menu[i]) + 3, 14, WHITE);
			}
		}
		flags.update_menu = 0;
	}
}

void MainSub1(uint8_t cmd)
{

}

void MainSub2(uint8_t cmd)
{
}

void MainSub3(uint8_t cmd)
{
}

void func201(uint8_t cmd)
{
}

void func202(uint8_t cmd)
{
}

void func301(uint8_t cmd)
{

}

void func302(uint8_t cmd)
{

}

void func401(uint8_t cmd)
{

}

void func402(uint8_t cmd)
{

}

//uint8_t* read_pgm_string( const char *FlashLoc )
//{
	//uint8_t tmp[128];
	//uint8_t i;
	//for(i = 0; (char)pgm_read_byte(&FlashLoc[i]); i++)
	//{
		//tmp [i] = (char)pgm_read_byte(&FlashLoc[i]);
	//}	
	//return &tmp[0];
//}

extern uint8_t in_fuel;
extern uint8_t out_fuel;
extern double total_fuel;
extern time_struct ts;

void MeasureMenu( uint8_t cmd )
{
	char tmp[20];
	uint8_t total;
	volatile double consumption;
	if(flags.update_fuel_values == 1)
	{
		ks0108ClearScreen();
		total = in_fuel - out_fuel;
		consumption = total / IMPULSES_PER_GRAM_SECOND;
		//ks0108GotoXY(3, 5);
		//ks0108FillRect(ks0108StringWidth("consumption  "), 5, 50, 10, WHITE);
		ks0108GotoXY(3, 5);
		sprintf(tmp, "consumption %.2f L/h", consumption);
		ks0108Puts(tmp);

		total_fuel += (consumption / 3600);
		//ks0108GotoXY(3, 25);
		//ks0108FillRect(ks0108StringWidth("total  "), 25, 50, 10, WHITE);
		ks0108GotoXY(3, 25);
		sprintf(tmp, "total %.2f L", (total_fuel));
		ks0108Puts(tmp);

		//ks0108GotoXY(3, 45);
		//ks0108FillRect(ks0108StringWidth("work time  "), 45, 50, 10, WHITE);
		ks0108GotoXY(3, 45);
		sprintf(tmp, "work time %02u:%02u:%02u", ts.hours, ts.minutes, ts.seconds);
		ks0108Puts(tmp);
		
		flags.update_fuel_values = 0;
	}
}

void set_menu( uint8_t menu_index )
{
	ks0108ClearScreen();
	MFPtr = (MenuFunctionPtr)pgm_read_word(&FuncPtrTable[menu_index]);	
}

void LongButtonPress()
{
	if(MN.menuNo == 0)
	{
		MN.menuNo = 1;
		set_menu(MN.menuNo);
	}
}

void ShortButtonPress(uint8_t button_index)
{
	if (MN.menuNo == 0)
	{
		return;
	}
	
	//if(button_index == BTN_UP)
	//{
	//if(MN.subMenuNo - 1 <= 0)
	//{
	//MN.subMenuNo = 0;
	//}
	//}
	//else if(button_index == BTN_DOWN)
	//{
	//
	//}
	//else if(button_index == BTN_RIGHT)
	//{
	//
	//}
	//else if(button_index == BTN_LEFT)
	//{
	//
	//}
	
	
	switch(button_index)
	{
		case BTN_UP:
		{
			if(MN.subMenuNo - 1 <= 0)
			{
				MN.subMenuNo = 0;
			}
			else
			{
				MN.subMenuNo--;
			}
		}
		break;
		case BTN_DOWN:
		{
			if(MN.subMenuNo + 1 >= 2)
			{
				MN.subMenuNo = 2;
			}
			else
			{
				MN.subMenuNo++;
			}
		}
		break;
		case BTN_RIGHT:
		break;
		case BTN_LEFT:
		break;
	}
}