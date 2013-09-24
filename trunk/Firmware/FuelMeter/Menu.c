/*
 * Menu.c
 *
 * Created: 31.08.2013 21:30:19
 *  Author: john
 */ 
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

#include "global.h"
#include "Menu.h"
#include "i2c_eeprom.h"
#include "Measurement.h"

#define MAX_MENU_LENGTH 32

#define SCREEN_ITEMS_COUNT 5;//(SCREEN_HEIGHT / SC_HEIGHT)

const char main_menu[][MAX_MENU_LENGTH] PROGMEM =
{
	{"VIEW HISTORY\0"},
	{"AVERAGE\0"},
	{"RESET\0"}
};

const char reset_menu[][MAX_MENU_LENGTH] PROGMEM =
{
	{"YES\0"},
	{"NO\0"},
};

//static volatile uint8_t menu_index = 0;
extern status_flags flags;

//Menu structure
const uint8_t menu_structure[] PROGMEM =
{
	0,
	3,	//number of main menu items
	1,	//Number of submenu items of menu item 1
	1,	//of menu item 2
	2,	//of menu reset
};

//Arrray of function pointers in Flash
const MenuFunctionPtr FuncPtrTable[] PROGMEM=
{
	MeasurePage,
	MainMenuPage,
	HistoryPage,
	AveragePage, 
	ResetPage,
};

extern uint8_t in_fuel;
extern uint8_t out_fuel;
extern double total_fuel;
extern time_struct ts;
static uint8_t offset;

volatile uint8_t current_menu_items_count;
extern volatile Menu_State MN;
static volatile bool update_menu = true;
extern volatile uint16_t total_records;

uint8_t place_header(char* header)
{
	ks0108SelectFont(SC, ks0108ReadFontData, BLACK);
	ks0108DrawRoundRect(0, 0, 127, SC_HEIGHT, 4, BLACK);
	ks0108GotoXY((SCREEN_WIDTH >> 1) - (ks0108StringWidth(header) >> 1), 1);
	ks0108Puts(header);	
	ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
	return SC_HEIGHT + 1;
}

void set_menu(uint8_t menu_index)
{
	ks0108ClearScreen();
	current_menu_items_count = pgm_read_byte(&menu_structure[menu_index]);
	MFPtr = (MenuFunctionPtr)pgm_read_word(&FuncPtrTable[menu_index]);	
	//ks0108ClearScreen();
	update_menu = true;
}

void menu_init(void)
{
	MN.menuNo = 0;
	MN.subMenuNo = 0;
	set_menu(MN.menuNo);
	update_menu = true;
}

void LongButtonPress()
{
	if(MN.menuNo == 0)
	{
		MN.menuNo = 1;
		MN.subMenuNo = 0;
		set_menu(MN.menuNo);
		update_menu = true;
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
				if(MN.subMenuNo == 0)
				{
					MN.menuNo = 2;
					MN.subMenuNo = 0;
					set_menu(MN.menuNo);
				}
				else if(MN.subMenuNo == 1)
				{
					MN.menuNo = 3;
					MN.subMenuNo = 0;
					set_menu(MN.menuNo);
					update_menu = true;
				}
				else if(MN.subMenuNo == 2)
				{
					MN.menuNo = 4;
					MN.subMenuNo = 1;
					set_menu(MN.menuNo);
				}
			}			
			else if(MN.menuNo == 4)
			{
				if(MN.subMenuNo == 0)
				{
					ResetHistory(button_index);
				}
				MN.menuNo = 1;
				MN.subMenuNo = 2;
				set_menu(MN.menuNo);
			}
		}
		break;
		
		case BTN_LEFT:
		{
			if(MN.menuNo == 4)
			{
				MN.menuNo = 1;
				MN.subMenuNo = 2;
				set_menu(MN.menuNo);
			}
			else if(MN.menuNo == 3)
			{
				MN.menuNo = 1;
				MN.subMenuNo = 1;
				set_menu(MN.menuNo);
			}
			else if(MN.menuNo == 2)
			{
				MN.menuNo = 1;
				MN.subMenuNo = 0;
				set_menu(MN.menuNo);
			}
			else
			{
				MN.menuNo = 0;
				MN.subMenuNo = 0;
				set_menu(MN.menuNo);
			}
		}
		break;
	}
	//flags.update_menu = 1;
	update_menu = true;
}

void MeasurePage( uint8_t cmd )
{
	char tmp[20];
	static uint8_t total;
	static double consumption;
//	static uint32_t addr = 0xABCDEF00;
//	static uint_fast32_t val = 0;
	if(flags.update_fuel_values == 1)
	{
		//ks0108ClearScreen();
		offset = place_header("MEASURE");
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
		
		//addr++;
		//eeWriteBytes(RECORDS_COUNT_ADDRESS, &addr, 4);
		//
		//ks0108SelectFont(SC, ks0108ReadFontData, BLACK);
		//offset += ARIAL_BOLD_14_HEIGHT - 4;
		//ks0108GotoXY(3, offset);
		//ks0108FillRect(ks0108StringWidth("ext eeprom: "), offset, 60, 10, WHITE);
		//ks0108GotoXY(3, offset);
		//eeReadBytes(RECORDS_COUNT_ADDRESS, &val, 4);
		//sprintf(tmp, "%"PRIX32" : %"PRIX32"", addr, val);
		//ks0108Puts(tmp);
		//ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
		
		flags.update_fuel_values = 0;
	}
}

void MainMenuPage( uint8_t cmd )
{
	//if(update_menu)
	{
		//ks0108ClearScreen();
		offset = place_header("MENU");
		//ks0108DrawRoundRect(0, 12, 127, 51, 4, BLACK);
		uint8_t y;
		for (uint8_t i = 0; i < current_menu_items_count; i++)
		{
			y = i * ARIAL_BOLD_14_HEIGHT + offset;
			ks0108GotoXY(5, y);
			ks0108Puts_P(main_menu[i]);
			if(MN.subMenuNo == i)
			{
				ks0108GotoXY(ks0108StringWidth_P(main_menu[i]) + 10, y);
				ks0108Puts(">");
				//ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, WHITE);
			}
			else
			{
				ks0108FillRect(ks0108StringWidth_P(main_menu[i]) + 10, y, 6, ARIAL_BOLD_14_HEIGHT, WHITE);				
				//ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
			}
		}
		//update_menu = false;
	}
}

void HistoryPage(uint8_t cmd)
{
	//if(update_menu)
	{
		//ks0108ClearScreen();
		offset = place_header("ViewHistory");
		//update_menu = false;
	}
}

void AveragePage( uint8_t cmd )
{
	char tmp[20];
	if(update_menu)
	{
		//ks0108ClearScreen();
		offset = place_header("AveragePage");
		ks0108FillRect(0, offset + 15, 127, 8, WHITE);
		ks0108DrawRect(0, offset + 5, 127, 8, BLACK);
		update_menu = false;
		measurement_struct* ms;
		double average_fuel = 0;
		float p = 127.0 / total_records;
		float width = 0;
		for (uint16_t i = 0; i < total_records; i++)
		{
			_delay_ms(500);
			//ms = &ReadMeasurement(i);
			//average_fuel += ms->total;
			width += p;
			ks0108FillRect(0, offset + 5, (int)width, 8, BLACK);


			ks0108FillRect(3, offset + 15, 127, ARIAL_BOLD_14_HEIGHT, WHITE);
			ks0108GotoXY(3, offset + 15);		
			sprintf(tmp, "complete %"PRIu16" of %"PRIu16"", i + 1, total_records);
			ks0108Puts(tmp);

		}
		
		ks0108FillRect(0, offset, 127, 48 - offset, WHITE);
		ks0108GotoXY(3, offset);
		//sprintf(tmp, "consumption 10.13 L/h");
		sprintf(tmp, "fuel rate 10.13 L/h");
		ks0108Puts(tmp);
		ks0108GotoXY(3, offset + ARIAL_BOLD_14_HEIGHT);
		sprintf(tmp, "total 100 L");
		ks0108Puts(tmp);
		ks0108GotoXY(3, offset + ARIAL_BOLD_14_HEIGHT + ARIAL_BOLD_14_HEIGHT);
		sprintf(tmp, "time 10:13:03");
		ks0108Puts(tmp);
	}
}

void ResetPage(uint8_t cmd)
{
	//uint16_t data = 0;
	//eeWriteBytes(RECORDS_COUNT_ADDRESS, (uint8_t*)&data, 2);
	char tmp[20];
	//if(update_menu)
	{
		//ks0108ClearScreen();
		offset = place_header("ResetPage");
		
		ks0108SelectFont(SC, ks0108ReadFontData, BLACK);
		ks0108GotoXY(3, offset + 5);
		
		sprintf(tmp, "Delete %"PRIu16" records?", total_records);
		ks0108Puts(tmp);
		
		//uint8_t y;
		for (uint8_t i = 0; i < current_menu_items_count; i++)
		{
			//y = i * ARIAL_BOLD_14_HEIGHT + offset;
			ks0108GotoXY(30 * i + 35, 40);
			if(MN.subMenuNo == i)
			{
				ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, WHITE);
				ks0108Puts_P(reset_menu[i]);
			}
			else
			{
				ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
				ks0108Puts_P(reset_menu[i]);
			}
		}


		//update_menu = false;
	}
}

void ResetHistory( uint8_t cmd )
{
	total_records = 0;
	eeWriteBytes(RECORDS_COUNT_ADDRESS, (uint8_t*)&total_records, 2);	
}
