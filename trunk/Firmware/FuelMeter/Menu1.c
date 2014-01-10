/*
 * Menu1.c
 *
 * Created: 30.11.2013 15:26:02
 *  Author: john
 
 список основных экранов:
 1. расход - мгновенный/средний за поездку
 2. остаток топлива/прогноз пробега
 3. скорость текущая/средняя/максимальная
 4. разгон до сотни, замер начинается после каждой остановки
 5. температура за бортом/в салоне (температура за бортом показывается на всех экранах)*
 6. время в пути
 7. напряжение бортовой сети
 */ 
#include "Menu1.h"

/////////////////////////////////////////////////////////////
//function prototypes
void DrawMainMenuBackground();
void MainMenuPage1(uint8_t cmd);
void MainMenuPage2(uint8_t cmd);
void MainMenuPage3(uint8_t cmd);
void MainMenuPage4(uint8_t cmd);
void MainMenuPage5(uint8_t cmd);
void MainMenuPage6(uint8_t cmd);
void MainMenuPage7(uint8_t cmd);
void SetMainMenuPage(uint8_t page_index);
void DrawTime();
//end function prototypes
/////////////////////////////////////////////////////////////

#define MAIN_PAGES_COUNT	7
uint8_t CurrentMainMenuIndex = 0;
typedef void (*MainMenuFuncPtr)(uint8_t cmd);
MainMenuFuncPtr CurrentMainMenuPage = NULL;
bool IsMainMenuMode = true;
volatile status_flags flags;
volatile time_struct ts;
volatile float peak_consumption = 0.0;
volatile float total_consumption = 0.0;


const MainMenuFuncPtr MainMenuFuncPtrTable[MAIN_PAGES_COUNT] PROGMEM=
{
	MainMenuPage1,
	MainMenuPage2,
	MainMenuPage3,
	MainMenuPage4,
	MainMenuPage5,
	MainMenuPage6,
	MainMenuPage7
};

void SetMainMenuPage(uint8_t page_index)
{
	CurrentMainMenuPage = (MainMenuFuncPtr)pgm_read_word(&MainMenuFuncPtrTable[page_index]);
	flags.update_menu = true;
}

void LongButtonPress(uint8_t button_index)
{
	
}

void ShortButtonPress(uint8_t button_index) 
{
	if (IsMainMenuMode)
	{
		switch (button_index)
		{
			case BTN_LEFT:
			{
				if(CurrentMainMenuIndex == 0)
				{
					CurrentMainMenuIndex = MAIN_PAGES_COUNT - 1;
				}
				else
				{
					--CurrentMainMenuIndex;
				}
				SetMainMenuPage(CurrentMainMenuIndex);
				break;
			}
			case BTN_RIGHT:
			{
				if(CurrentMainMenuIndex == MAIN_PAGES_COUNT - 1)
				{
					CurrentMainMenuIndex = 0;
				}
				else
				{
					++CurrentMainMenuIndex;
				}
				SetMainMenuPage(CurrentMainMenuIndex);
				break;
			}
			case BTN_DOWN:
			{
				break;
			}
			case BTN_UP:
			{
				break;
			}
		}
	} 
	else
	{
	}
}

void DrawTime()
{
	displaySelectFont(fixednums15x31, TFT_WHITE);	
	TFT_drawNumber(ts.hours, 50, 10, 1, TFT_WHITE, 0);
	TFT_drawString(":", 60, 10, 1, TFT_WHITE);
	TFT_drawNumber(ts.minutes, 70, 10, 1, TFT_WHITE, 0);
}

void DrawMainMenuBackground()
{
	//displaySelectFont(fixednums15x31, TFT_WHITE);
	//uint_fast16_t width = TFT_StringWidth("14:02");
	//TFT_drawString("14:02", 120 - (width >> 1), 10, 1, TFT_WHITE);
	TFT_fillRectangle(10, 47, MAX_X - 20, 3, TFT_WHITE);

	TFT_fillRectangle(10, 226, MAX_X - 20, 3, TFT_WHITE);

	
	TFT_fillRectangle(10, 268, MAX_X - 20, 3, TFT_WHITE);
	//TFT_drawNumber(width >> 1, 10, 270, 1, TFT_WHITE, 0);
}

static float prev_peak_consumption = 0;

void MainMenuPage1(uint8_t cmd)
{
	if(flags.update_menu)
	{
		DrawMainMenuBackground();
		flags.update_menu = 0;
	}

	if (flags.update_consumption)
	{
		DrawTime();
		displaySelectFont(ArialNums40x37, TFT_WHITE);
		TFT_drawFloat(prev_peak_consumption, 2, 20, 60, 2, TFT_BLACK);
		TFT_drawFloat(peak_consumption, 2, 20, 60, 2, TFT_WHITE);
		prev_peak_consumption = peak_consumption;
		flags.update_consumption = false;
	}
	
}

void MainMenuPage2(uint8_t cmd)
{
	//displaySelectFont(SystemRus5x7, 0xffff);
	TFT_fillRectangle(10, 230, MAX_X - 20, 38, TFT_BLACK);
	TFT_drawString("MainMenuPage2", 10, 230, 1, TFT_WHITE);
}

void MainMenuPage3(uint8_t cmd)
{
	//displaySelectFont(SystemRus5x7, 0xffff);
	TFT_fillRectangle(10, 230, MAX_X - 20, 38, TFT_BLACK);
	TFT_drawString("MainMenuPage3", 10, 230, 1, TFT_WHITE);
}

void MainMenuPage4(uint8_t cmd)
{
	//displaySelectFont(SystemRus5x7, 0xffff);
	TFT_fillRectangle(10, 230, MAX_X - 20, 38, TFT_BLACK);
	TFT_drawString("MainMenuPage4", 10, 230, 1, TFT_WHITE);
}

void MainMenuPage5(uint8_t cmd)
{
	//displaySelectFont(SystemRus5x7, 0xffff);
	TFT_fillRectangle(10, 230, MAX_X - 20, 38, TFT_BLACK);
	TFT_drawString("MainMenuPage5", 10, 230, 1, TFT_WHITE);
}

void MainMenuPage6(uint8_t cmd)
{
	//displaySelectFont(SystemRus5x7, 0xffff);
	TFT_fillRectangle(10, 230, MAX_X - 20, 38, TFT_BLACK);
	TFT_drawString("MainMenuPage6", 10, 230, 1, TFT_WHITE);
}

void MainMenuPage7(uint8_t cmd)
{
	//displaySelectFont(SystemRus5x7, 0xffff);
	TFT_fillRectangle(10, 230, MAX_X - 20, 38, TFT_BLACK);
	TFT_drawString("MainMenuPage7", 10, 230, 1, TFT_WHITE);
}

void ProcessMenu( uint8_t cmd )
{
	//if (flags.update_menu)
	{
		//TFT_fillScreen(0, 239, 0, 319, TFT_BLACK);
		CurrentMainMenuPage(cmd);
		//flags.update_menu = false;
	}
}

void InitMenu()
{
	displayClear();
	SetMainMenuPage(CurrentMainMenuIndex);
}
