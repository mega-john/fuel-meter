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
	MainSub1,	//functions for submenus of menu 1
	func201, func202, func203, 			//functions for submenus of menu 2
	func301, func302, 			//functions for submenus of menu 3
};

volatile uint8_t menu_stack_pointer;
volatile uint8_t menu_stack[10];

extern uint8_t in_fuel;
extern uint8_t out_fuel;
extern double total_fuel;
extern time_struct ts;

volatile uint8_t current_menu_items_count;

void set_menu(uint8_t menu_index)
{
	ks0108ClearScreen();
	current_menu_items_count = pgm_read_byte(&menu_structure[menu_index]);
	MFPtr = (MenuFunctionPtr)pgm_read_word(&FuncPtrTable[menu_index]);	
	flags.update_menu = 1;
}

void menu_init(void)
{
	menu_stack_pointer = 0;
	MN.menuNo = 0;
	MN.subMenuNo = 0;
	set_menu(MN.menuNo);
}

//uint8_t MFIndex(uint8_t mn, uint8_t sb)
//{
	//uint8_t p = 0;//points to menu in table of function pointer
	//for(uint8_t i = 0; i < (mn - 1); i++)
	//{
		//p = p + pgm_read_byte(&menu_structure[i + 1]);
	//}
	//p = p + sb - 1;
	//return p;
//}
//

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
	
	switch(button_index)
	{
		case BTN_UP:
		{
			MN.subMenuNo--;
			if(MN.subMenuNo == 255)
			{
				MN.subMenuNo = current_menu_items_count - 1;
			}
		}
		break;
		
		case BTN_DOWN:
		{
			MN.subMenuNo++;
			if(MN.subMenuNo >= current_menu_items_count)
			{
				MN.subMenuNo = 0;
			}
		}
		break;
		
		case BTN_RIGHT:
		{			
			if(MN.menuNo == 1)
			{
				MN.menuNo = 2 + MN.subMenuNo;
				MN.subMenuNo = 0;
				set_menu(MN.menuNo);
			}
			//if(MN.menuNo == 2)
			//{
				//MN.menuNo = 2 + MN.subMenuNo;
				//set_menu(MN.menuNo);
			//}
		}
		break;
		
		case BTN_LEFT:
		{
			
		}
		break;
	}
}

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

void MainMenu( uint8_t cmd )
{
	if(flags.update_menu == 1)
	{
		for (uint8_t i = 0; i < current_menu_items_count; i++)
		{
			ks0108GotoXY(2, i * 15 + 1);
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
	if(flags.update_menu == 1)
	{
		for (uint8_t i = 0; i < current_menu_items_count; i++)
		{
			ks0108GotoXY(2, i * 15 + 1);
			ks0108Puts_P(sub_menu_0[i]);
			if(MN.subMenuNo == i)
			{
				ks0108DrawRect(0, i * 15, ks0108StringWidth_P(sub_menu_0[i]) + 3, 14, BLACK);
			}
			else
			{
				ks0108DrawRect(0, i * 15, ks0108StringWidth_P(sub_menu_0[i]) + 3, 14, WHITE);
			}
		}
		flags.update_menu = 0;
	}
}

void func201(uint8_t cmd)
{
	if(flags.update_menu == 1)
	{
		for (uint8_t i = 0; i < current_menu_items_count; i++)
		{
			ks0108GotoXY(2, i * 15 + 1);
			ks0108Puts_P(sub_menu_1[i]);
			if(MN.subMenuNo == i)
			{
				ks0108DrawRect(0, i * 15, ks0108StringWidth_P(sub_menu_1[i]) + 3, 14, BLACK);
			}
			else
			{
				ks0108DrawRect(0, i * 15, ks0108StringWidth_P(sub_menu_1[i]) + 3, 14, WHITE);
			}
		}
		flags.update_menu = 0;
	}
}

void func202(uint8_t cmd)
{
}

void func203(uint8_t cmd)
{
}

void func301(uint8_t cmd)
{
}

void func302(uint8_t cmd)
{
}
