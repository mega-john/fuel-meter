/*
 * Menu.c
 *
 * Created: 31.08.2013 21:30:19
 *  Author: john
 */ 
#include "menu.h"

volatile uint8_t fuel_impulses;
volatile uint8_t distance_impulses;

extern time_struct ts;
static uint8_t offset;

volatile uint8_t current_menu_items_count;
extern volatile Menu_State MN;
static volatile bool update_menu = true;
extern volatile uint16_t total_measurements;
measurement_struct ms;
uint8_t DateTime[7];

const char main_menu[][MAX_MENU_LENGTH] PROGMEM =
{
	{"VIEW HISTORY\0"},
	{"AVERAGE\0"},
	{"RESET\0"}
};

const char reset_menu[][MAX_MENU_LENGTH] PROGMEM =
{
	{" YES \0"},
	{" NO \0"},
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

inline uint32_t ToSeconds(time_struct* ts)
{
	return ts->seconds + ts->minutes*60 + ts->hours*3600;
}

uint8_t place_header(char* header)
{
	ks0108SelectFont(SystemRus5x7, ks0108ReadFontData, BLACK);
	ks0108DrawRoundRect(0, 0, 127, SYSTEMRUS5x7_HEIGHT, 4, BLACK);
	ks0108GotoXY((SCREEN_WIDTH >> 1) - (ks0108StringWidth(header) >> 1), 1);
	ks0108Puts(header);	
	//ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
	return SYSTEMRUS5x7_HEIGHT;
}

void set_menu(uint8_t menu_index)
{
	ks0108ClearScreen();
	current_menu_items_count = pgm_read_byte(&menu_structure[menu_index]);
	MFPtr = (MenuFunctionPtr)pgm_read_word(&FuncPtrTable[menu_index]);	
	//ks0108ClearScreen();
	//update_menu = true;
}

void menu_init(void)
{
	ks0108SelectFont(SystemRus5x7, ks0108ReadFontData, BLACK);	
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
				update_menu = false;
				set_menu(MN.menuNo);
				return;
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
	update_menu = true;
}

void DrawGrid() 
{
	ks0108DrawRect(0, 0, 127, SYSTEMRUS5x7_HEIGHT, BLACK);
	ks0108DrawRect(0, SYSTEMRUS5x7_HEIGHT, 69, 63 - SYSTEMRUS5x7_HEIGHT, BLACK);
	ks0108DrawRect(69, SYSTEMRUS5x7_HEIGHT, 58, SYSTEMRUS5x7_HEIGHT, BLACK);
	ks0108DrawRect(69, SYSTEMRUS5x7_HEIGHT * 2, 29, SYSTEMRUS5x7_HEIGHT, BLACK);
	ks0108DrawRect(98, SYSTEMRUS5x7_HEIGHT * 2, 29, SYSTEMRUS5x7_HEIGHT, BLACK);
	ks0108DrawRect(69, SYSTEMRUS5x7_HEIGHT * 3, 58, 63 - SYSTEMRUS5x7_HEIGHT * 3, BLACK);
}

void MeasurePage( uint8_t cmd )
{
	if(flags.update_fuel_values == 1)
	{		
		char tmp[20];
	
		DrawGrid();

		//draw date/time
		ds1703_read((uint8_t*)&DateTime);
		ks0108FillRect(1, 1, 125, SYSTEMRUS5x7_HEIGHT - 2, WHITE);
		sprintf(tmp, "%02i/%02i/%02i-%02i:%02i:%02i", DateTime[4], DateTime[5], DateTime[6], DateTime[2], DateTime[1], DateTime[0]);
		ks0108GotoXY(12, 2);
		ks0108Puts(tmp);
		
		//draw fuel consumption
		double consumption = 0.0;
		static double total_fuel = 0.0;
		consumption = fuel_impulses * 0.36;/// IMPULSES_PER_GRAM_SECOND;
		total_fuel += (fuel_impulses * 0.0001);
		ks0108FillRect(70, SYSTEMRUS5x7_HEIGHT + 1, 56, SYSTEMRUS5x7_HEIGHT - 2, WHITE);
		sprintf(tmp, "%.3f", consumption);
		ks0108GotoXY(71, SYSTEMRUS5x7_HEIGHT + 2);
		ks0108Puts(tmp);
		
		//draw temperature
		ks0108FillRect(70, SYSTEMRUS5x7_HEIGHT * 2 + 1, 27, SYSTEMRUS5x7_HEIGHT - 3, WHITE);
		sprintf(tmp, "36.6", consumption);
		ks0108GotoXY(71, SYSTEMRUS5x7_HEIGHT * 2 + 2);
		ks0108Puts(tmp);
		
		ks0108FillRect(99, SYSTEMRUS5x7_HEIGHT * 2 + 1, 27, SYSTEMRUS5x7_HEIGHT - 3, WHITE);
		sprintf(tmp, "36.6", consumption);
		ks0108GotoXY(100, SYSTEMRUS5x7_HEIGHT * 2 + 2);
		ks0108Puts(tmp);
		
		
		
		//
		////total_fuel += (consumption / 3600);
		//offset += SYSTEMRUS5x7_HEIGHT - 2;
		//ks0108GotoXY(3, offset);
		//ks0108FillRect(ks0108StringWidth("total: "), offset, 60, 10, WHITE);
		//ks0108GotoXY(3, offset);
		//sprintf(tmp, "total: %.3f L", total_fuel);
		//ks0108Puts(tmp);
//
		//offset += SYSTEMRUS5x7_HEIGHT - 2;
		//ks0108GotoXY(3, offset);
		//ks0108FillRect(ks0108StringWidth("time:"), offset, 60, 10, WHITE);
		//ks0108GotoXY(3, offset);
		//sprintf(tmp, "time: %02u:%02u:%02u", ts.hours, ts.minutes, ts.seconds);
		//ks0108Puts(tmp);
		//
		//
		////addr++;
		//eeWriteBytes(RECORDS_COUNT_ADDRESS, &addr, 4);
		//
	//ks0108SelectFont(SC, ks0108ReadFontData, BLACK);
		//offset += ARIAL_BOLD_14_HEIGHT - 4;
		//ks0108GotoXY(3, offset);
		//ks0108FillRect(ks0108StringWidth("size: "), offset, 60, 10, WHITE);
		//ks0108GotoXY(3, offset);
		////eeReadBytes(RECORDS_COUNT_ADDRESS, &val, 4);
		//sprintf(tmp, "size: %i", MEASUREMENT_STRUCT_SIZE);
		//ks0108Puts(tmp);
		//ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
		
		flags.update_fuel_values = 0;
	}
}

void MainMenuPage( uint8_t cmd )
{
	//if(update_menu)
	{
		//update_menu = false;
		offset = place_header("menu");
		//ks0108DrawRoundRect(0, 12, 127, 51, 4, BLACK);
		uint8_t y;
		for (uint8_t i = 0; i < current_menu_items_count; i++)
		{
			y = i * SYSTEMRUS5x7_HEIGHT + offset;
			ks0108GotoXY(5, y);
			ks0108Puts_P(main_menu[i]);
			if(MN.subMenuNo == i)
			{
				ks0108GotoXY(ks0108StringWidth_P(main_menu[i]) + 10, y);
				ks0108Puts(">");
				//ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, WHITE);
				//ks0108Puts_P(main_menu[i]);
			}
			else
			{
				ks0108FillRect(ks0108StringWidth_P(main_menu[i]) + 10, y, 6, SYSTEMRUS5x7_HEIGHT, WHITE);				
				//ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
				//ks0108Puts_P(main_menu[i]);
			}
		}
	}
}

void HistoryPage(uint8_t cmd)
{
	if(update_menu)
	{
		update_menu = false;
		offset = place_header("view history");
		if(total_measurements <= 0)
		{
			ks0108GotoXY(3, offset);
			ks0108Puts("no history!");
			return;
		}
	}
}

void AveragePage( uint8_t cmd )
{
	if(update_menu)
	{
		update_menu = false;

		offset = place_header("average page");
		if(total_measurements <= 0)
		{
			ks0108GotoXY(3, offset);
			ks0108Puts("no history!");
			return;
		}
		
		char tmp[20];
		ks0108FillRect(0, offset + 15, 127, 8, WHITE);
		ks0108DrawRect(0, offset + 5, 127, 8, BLACK);
		double avearge_history_fuel = 0.0;
		double history_fuel = 0.0;
		uint32_t seconds_history = 0;
		uint32_t minutes_history = 0;
		uint32_t hours_history = 0;
		uint32_t days_history = 0;
		float p = 127.0 / total_measurements;
		float width = 0.0;
		for (uint16_t i = 0; i < total_measurements; i++)
		{
			ReadMeasurement(i, &ms) ;
			history_fuel += ms.total;
			seconds_history += ms.time.seconds;
			minutes_history += ms.time.minutes;
			hours_history += ms.time.hours;
			
			width += p;
			ks0108FillRect(0, offset + 5, (int)width, 8, BLACK);
			ks0108FillRect(3, offset + 15, 127, SYSTEMRUS5x7_HEIGHT, WHITE);
			ks0108GotoXY(3, offset + 15);		
			sprintf(tmp, "complete %"PRIu16" of %"PRIu16"", i + 1, total_measurements);
			ks0108Puts(tmp);
		}
		
		minutes_history += seconds_history / 60;
		seconds_history = seconds_history % 60;
		
		hours_history += minutes_history / 60;
		minutes_history = minutes_history % 60;
		
		days_history += hours_history / 24;
		hours_history = hours_history % 24;
		
		avearge_history_fuel = (double)(history_fuel / ((days_history * 24 + hours_history) + ((minutes_history * 60 + seconds_history) / 3600.0)));

		ks0108FillRect(0, offset, 127, 48 - offset, WHITE);
		
		ks0108GotoXY(3, offset);
		sprintf(tmp, "fuel rate: %.2f L/h", avearge_history_fuel);
		ks0108Puts(tmp);
		
		ks0108GotoXY(3, offset + SYSTEMRUS5x7_HEIGHT);
		sprintf(tmp, "total: %.2f L", history_fuel);
		ks0108Puts(tmp);
		
		ks0108GotoXY(3, offset + SYSTEMRUS5x7_HEIGHT + SYSTEMRUS5x7_HEIGHT);
		sprintf(tmp, "time %lu - %02lu:%02lu:%02lu", days_history, hours_history, minutes_history, seconds_history);
		ks0108Puts(tmp);
	}
}

void ResetPage(uint8_t cmd)
{
	if(update_menu)
	{
		update_menu = false;

		offset = place_header("reset page");
		if(total_measurements <= 0)
		{
			ks0108GotoXY(3, offset);
			ks0108Puts("no history!");
			return;
		}
		
		char tmp[20];
		ks0108SelectFont(SystemRus5x7, ks0108ReadFontData, BLACK);
		ks0108GotoXY(3, offset + 5);
		
		sprintf(tmp, "Delete %"PRIu16" records?", total_measurements);
		ks0108Puts(tmp);
		
		for (uint8_t i = 0; i < current_menu_items_count; i++)
		{
			ks0108GotoXY(35 * i + 25, 40);
			if(MN.subMenuNo == i)
			{
				ks0108SelectFont(SystemRus5x7, ks0108ReadFontData, WHITE);
				ks0108Puts_P(reset_menu[i]);
			}
			else
			{
				ks0108SelectFont(SystemRus5x7, ks0108ReadFontData, BLACK);
				ks0108Puts_P(reset_menu[i]);
			}
		}
	}
}

void ResetHistory( uint8_t cmd )
{
	total_measurements = 0;
//	eeWriteBytes(RECORDS_COUNT_ADDRESS, (uint8_t*)&total_measurements, 2);	
}
