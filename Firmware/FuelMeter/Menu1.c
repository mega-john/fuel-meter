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

static bool flash = false;

void DrawTime()
{
	char tmp[10];
	
	sprintf(tmp, "%02d:%02d:%02d", ts.hours, ts.minutes, ts.seconds);
	
	//displaySelectFont(fixednums15x31, TFT_WHITE);	
	displaySelectFont(SystemRus5x7);
	TFT_drawString(tmp, 50, 10, 2, TFT_WHITE, TFT_BLACK);
	//TFT_drawNumber(ts.hours, 50, 10, 1, TFT_WHITE, TFT_BLACK, 0);
	//TFT_drawString(":", 80, 10, 1, TFT_WHITE, TFT_BLACK);
	//TFT_drawNumber(ts.minutes, 110, 10, 1, TFT_WHITE, TFT_BLACK, 0);
}

void DrawMainMenuBackground()
{
	TFT_fillRectangle(10, 47, MAX_X - 20, 3, TFT_WHITE);
	TFT_fillRectangle(10, 226, MAX_X - 20, 3, TFT_WHITE);
	TFT_fillRectangle(10, 268, MAX_X - 20, 3, TFT_WHITE);
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
		displaySelectFont(fixednums15x31);
		//TFT_drawFloat(prev_peak_consumption, 2, 30, 60, 1, TFT_BLACK);
		TFT_drawFloat(peak_consumption, 2, 30, 60, 1, TFT_WHITE, TFT_BLACK);
		flash = !flash;
		prev_peak_consumption = peak_consumption;
		flags.update_consumption = false;
	}
	
}

void MainMenuPage2(uint8_t cmd)
{
	//displaySelectFont(SystemRus5x7, 0xffff);
	TFT_fillRectangle(10, 230, MAX_X - 20, 38, TFT_BLACK);
	TFT_drawString("MainMenuPage2", 10, 230, 1, TFT_WHITE, TFT_BLACK);
}

void MainMenuPage3(uint8_t cmd)
{
	//displaySelectFont(SystemRus5x7, 0xffff);
	TFT_fillRectangle(10, 230, MAX_X - 20, 38, TFT_BLACK);
	TFT_drawString("MainMenuPage3", 10, 230, 1, TFT_WHITE, TFT_BLACK);
}

void MainMenuPage4(uint8_t cmd)
{
	//displaySelectFont(SystemRus5x7, 0xffff);
	TFT_fillRectangle(10, 230, MAX_X - 20, 38, TFT_BLACK);
	TFT_drawString("MainMenuPage4", 10, 230, 1, TFT_WHITE, TFT_BLACK);
}

void MainMenuPage5(uint8_t cmd)
{
	//displaySelectFont(SystemRus5x7, 0xffff);
	TFT_fillRectangle(10, 230, MAX_X - 20, 38, TFT_BLACK);
	TFT_drawString("MainMenuPage5", 10, 230, 1, TFT_WHITE, TFT_BLACK);
}

void MainMenuPage6(uint8_t cmd)
{
	//displaySelectFont(SystemRus5x7, 0xffff);
	TFT_fillRectangle(10, 230, MAX_X - 20, 38, TFT_BLACK);
	TFT_drawString("MainMenuPage6", 10, 230, 1, TFT_WHITE, TFT_BLACK);
}

void MainMenuPage7(uint8_t cmd)
{
	//displaySelectFont(SystemRus5x7, 0xffff);
	TFT_fillRectangle(10, 230, MAX_X - 20, 38, TFT_BLACK);
	TFT_drawString("MainMenuPage7", 10, 230, 1, TFT_WHITE, TFT_BLACK);
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
