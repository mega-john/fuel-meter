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

//const char main_menu[][MAX_MENU_LENGTH] PROGMEM =
//{
////	{"1 measure\0"},
	//{"1 history \0"},
////	{"3 calibrate\0"}
//};
//
//const char sub_menu_0[][MAX_MENU_LENGTH] PROGMEM =
//{
	//{"11 real time\0"},
//};

const char history_menu[][MAX_MENU_LENGTH] PROGMEM =
{
//	{"history\0"},
	{"view\0"},
	{"average\0"},
	{"reset\0"}
};

//const char sub_menu_2[][MAX_MENU_LENGTH] PROGMEM =
//{
	//{"31 default\0"},
	//{"32 start\0"},
//};

//static volatile uint8_t menu_index = 0;
extern status_flags flags;

//Menu structure
const uint8_t menu_structure[] PROGMEM =
{
	0,
	3,	//number of main menu items
	//1,	//Number of submenu items of menu item 1
	//3,	//of menu item 2
	//2,	//of menu item 3
};

//Arrray of function pointers in Flash
const MenuFunctionPtr FuncPtrTable[] PROGMEM=
{
	MeasurePage,
	HistoryPageMenu,
		ViewHistory,
		AverageHistory, 
		ResetHistory
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
	//ks0108ClearScreen();
	current_menu_items_count = pgm_read_byte(&menu_structure[menu_index]);
	MFPtr = (MenuFunctionPtr)pgm_read_word(&FuncPtrTable[menu_index]);	
	flags.update_menu = 1;
	ks0108ClearScreen();
}

void menu_init(void)
{
	menu_stack_pointer = 0;
	MN.menuNo = 0;
	MN.subMenuNo = 0;
	set_menu(MN.menuNo);
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
	
	switch(button_index)
	{
		case BTN_UP:
		{
			MN.subMenuNo--;
			if(MN.subMenuNo >= current_menu_items_count)
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
			MN.menuNo = 0;
			MN.subMenuNo = 0;
			set_menu(MN.menuNo);
		}
		break;
	}
}

void MeasurePage( uint8_t cmd )
{
	char tmp[20];
	static uint8_t total;
	static double consumption;
	if(flags.update_fuel_values == 1)
	{
		//ks0108ClearScreen();
		ks0108DrawRoundRect(0, 0, 127, 14, 4, BLACK);
		ks0108GotoXY((SCREEN_WIDTH >> 1) - (ks0108StringWidth("measure") >> 1), 0);
		ks0108Puts("measure");
		//ks0108DrawRoundRect(0, 12, 127, 51, 4, BLACK);
		total = in_fuel - out_fuel;
		consumption = total / IMPULSES_PER_GRAM_SECOND;
		//ks0108GotoXY(3, 5);
		//ks0108FillRect(ks0108StringWidth("CURRENT: "), 5, 50, 10, WHITE);
		ks0108GotoXY(3, 19);
		sprintf(tmp, "current: %.2f L/h", consumption);
		ks0108Puts(tmp);

		total_fuel += (consumption / 3600);
		//ks0108GotoXY(3, 18);
		//ks0108FillRect(ks0108StringWidth("TOTAL: "), 18, 50, 10, WHITE);
		ks0108GotoXY(3, 32);
		sprintf(tmp, "total: %.2f L", (total_fuel));
		ks0108Puts(tmp);

		//ks0108GotoXY(3, 31);
		//ks0108FillRect(ks0108StringWidth("Work time: "), 31, 50, 10, WHITE);
		ks0108GotoXY(3, 45);
		sprintf(tmp, "time: %02u:%02u:%02u", ts.hours, ts.minutes, ts.seconds);
		ks0108Puts(tmp);
		
		flags.update_fuel_values = 0;
	}
}

void HistoryPageMenu( uint8_t cmd )
{
	if(flags.update_menu == 1)
	{
		ks0108ClearScreen();
		ks0108DrawRoundRect(0, 0, 127, 14, 4, BLACK);
		ks0108GotoXY((SCREEN_WIDTH >> 1) - (ks0108StringWidth("history") >> 1), 1);
		ks0108Puts("history");
		//ks0108DrawRoundRect(0, 12, 127, 51, 4, BLACK);

		for (uint8_t i = 0; i < current_menu_items_count; i++)
		{
			ks0108GotoXY(5, i * 14 + 15);
			ks0108Puts_P(history_menu[i]);
			if(MN.subMenuNo == i)
			{
				ks0108GotoXY(ks0108StringWidth_P(history_menu[i]) + 10, i * 14 + 15);
				ks0108Puts(">");
			}
			else
			{
				ks0108FillRect(ks0108StringWidth_P(history_menu[i]) + 10, i * 14 + 15, 6, 15, WHITE);				
			}
		}
		flags.update_menu = 0;
	}
}

void ViewHistory(uint8_t cmd)
{
	if(flags.update_menu == 1)
	{
		for (uint8_t i = 0; i < current_menu_items_count; i++)
		{
			//ks0108GotoXY(2, i * 15 + 1);
			//ks0108Puts_P(sub_menu_0[i]);
			//if(MN.subMenuNo == i)
			//{
				//ks0108DrawRect(0, i * 15, ks0108StringWidth_P(sub_menu_0[i]) + 3, 14, BLACK);
			//}
			//else
			//{
				//ks0108DrawRect(0, i * 15, ks0108StringWidth_P(sub_menu_0[i]) + 3, 14, WHITE);
			//}
		}
		flags.update_menu = 0;
	}
}

void AverageHistory(uint8_t cmd)
{
	if(flags.update_menu == 1)
	{
		for (uint8_t i = 0; i < current_menu_items_count; i++)
		{
			ks0108GotoXY(2, i * 15 + 1);
			ks0108Puts_P(history_menu[i]);
			if(MN.subMenuNo == i)
			{
				ks0108DrawRect(0, i * 15, ks0108StringWidth_P(history_menu[i]) + 3, 14, BLACK);
			}
			else
			{
				ks0108DrawRect(0, i * 15, ks0108StringWidth_P(history_menu[i]) + 3, 14, WHITE);
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

void ResetHistory(uint8_t cmd)
{
}
