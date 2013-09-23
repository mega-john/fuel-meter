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

#define SCREEN_ITEMS_COUNT 5;//(SCREEN_HEIGHT / SC_HEIGHT)

const char history_menu[][MAX_MENU_LENGTH] PROGMEM =
{
	{"VIEW\0"},
	{"AVERAGE\0"},
	{"RESET\0"}
};

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
			HistoryPage,
		AverageHistory, 
			AveragePage,
		ResetHistory
};

volatile uint8_t menu_stack_pointer;
volatile uint8_t menu_stack[10];

extern uint8_t in_fuel;
extern uint8_t out_fuel;
extern double total_fuel;
extern time_struct ts;
static uint8_t offset;

volatile uint8_t current_menu_items_count;
extern volatile Menu_State MN;

uint8_t place_header(const char* header)
{
	ks0108SelectFont(SC, ks0108ReadFontData, BLACK);
	ks0108DrawRoundRect(0, 0, 127, SC_HEIGHT - 1, 4, BLACK);
	ks0108GotoXY((SCREEN_WIDTH >> 1) - (ks0108StringWidth(header) >> 1), 1);
	ks0108Puts(header);	
	ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
	return SC_HEIGHT;
}

void set_menu(uint8_t menu_index)
{
	current_menu_items_count = pgm_read_byte(&menu_structure[menu_index]);
	MFPtr = (MenuFunctionPtr)pgm_read_word(&FuncPtrTable[menu_index]);	
	ks0108ClearScreen();
	flags.update_menu = 1;
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
		flags.update_menu = 0;
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
	flags.update_menu = 1;
}

void MeasurePage( uint8_t cmd )
{
	char tmp[20];
	static uint8_t total;
	static double consumption;
	static uint16_t addr = RECORDS_COUNT_ADDRESS;
	static uint8_t val;
	if(flags.update_fuel_values == 1)
	{
		offset = place_header("MEASURE");
		//ks0108ClearScreen();
		//ks0108DrawRoundRect(0, 12, 127, 51, 4, BLACK);
		total = in_fuel - out_fuel;
		consumption = total / IMPULSES_PER_GRAM_SECOND;
		ks0108GotoXY(3, offset);
		ks0108FillRect(ks0108StringWidth("current: "), offset, 60, 10, WHITE);
		ks0108GotoXY(3, offset);
		sprintf(tmp, "current: %.2f L/h", consumption);
		ks0108Puts(tmp);
			
		total_fuel += (consumption / 3600);
		offset += ARIAL_BOLD_14_HEIGHT - 2;
		ks0108GotoXY(3, offset);
		ks0108FillRect(ks0108StringWidth("total: "), offset, 60, 10, WHITE);
		ks0108GotoXY(3, offset);
		sprintf(tmp, "total: %.2f L", (total_fuel));
		ks0108Puts(tmp);

		offset += ARIAL_BOLD_14_HEIGHT - 2;
		ks0108GotoXY(3, offset);
		ks0108FillRect(ks0108StringWidth("time:"), offset, 60, 10, WHITE);
		ks0108GotoXY(3, offset);
		sprintf(tmp, "time: %02u:%02u:%02u", ts.hours, ts.minutes, ts.seconds);
		ks0108Puts(tmp);
		
		ks0108SelectFont(SC, ks0108ReadFontData, BLACK);
		offset += ARIAL_BOLD_14_HEIGHT - 4;
		ks0108GotoXY(3, offset);
		ks0108FillRect(ks0108StringWidth("ext eeprom: "), offset, 60, 10, WHITE);
		ks0108GotoXY(3, offset);
		eeReadByte(addr, &val);
		sprintf(tmp, "ext eeprom: %02u:%i", addr++, val++);
		ks0108Puts(tmp);
		ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
		
		flags.update_fuel_values = 0;
	}
}

void HistoryPageMenu( uint8_t cmd )
{
	if(flags.update_menu == 1)
	{
		ks0108ClearScreen();
		offset = place_header("MENU");
		//ks0108DrawRoundRect(0, 12, 127, 51, 4, BLACK);
		uint8_t y;
		for (uint8_t i = 0; i < current_menu_items_count; i++)
		{
			y = i * ARIAL_BOLD_14_HEIGHT + offset;
			ks0108GotoXY(5, y);
			ks0108Puts_P(history_menu[i]);
			if(MN.subMenuNo == i)
			{
				ks0108GotoXY(ks0108StringWidth_P(history_menu[i]) + 10, y);
				ks0108Puts(">");
				//ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, WHITE);
			}
			else
			{
				ks0108FillRect(ks0108StringWidth_P(history_menu[i]) + 10, y, 6, ARIAL_BOLD_14_HEIGHT, WHITE);				
				//ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
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

void HistoryPage( uint8_t cmd )
{
	uint16_t items_count = 0; 
	if(eeReadByte(RECORDS_COUNT_ADDRESS, &items_count, 2))
	{
		
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

void AveragePage( uint8_t cmd )
{
}

void ResetHistory(uint8_t cmd)
{
}
