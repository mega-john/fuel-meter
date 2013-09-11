/*
 * Menu.c
 *
 * Created: 31.08.2013 21:30:19
 *  Author: john
 */ 
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

volatile static char menu_index = 0;
extern status_flags flags;


//Menu structure
//[0] -Number of level 0 menu items
//[1]...[n] number of second level menu items
//Eg. MSTR2[1] shows that menu item 1 has 3 submenus
const char menu_structure[] PROGMEM =
{
	3,	//number of menu items
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

const char MenuIndexes[] PROGMEM = 
{
	0, 1, 2, 3,
	10, 11, 12 ,13,
	20, 21, 22, 23,	
};

void menu_Init(void)
{
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

void MainMenu( char cmd )
{
	if(flags.update_menu == 1)
	{
		ks0108ClearScreen();
		for (char i = 0; i < 3; i++)
		{
			ks0108GotoXY(2, i * 14 + 1);
			ks0108Puts_P(main_menu[i]);
			if(MN.menuNo == i)
			{
				ks0108DrawRect(0, i * 14, ks0108StringWidth_P(main_menu[i]) + 3, 14, BLACK);
			}
		}
		flags.update_menu = 0;
	}
}

void MainSub1(char cmd)
{

}

void MainSub2(char cmd)
{
}

void MainSub3(char cmd)
{
}

void func201(char cmd)
{
}

void func202(char cmd)
{
}

void func301(char cmd)
{

}

void func302(char cmd)
{

}

void func401(char cmd)
{

}

void func402(char cmd)
{

}

//unsigned char* read_pgm_string( const char *FlashLoc )
//{
	//unsigned char tmp[128];
	//uint8_t i;
	//for(i = 0; (char)pgm_read_byte(&FlashLoc[i]); i++)
	//{
		//tmp [i] = (char)pgm_read_byte(&FlashLoc[i]);
	//}	
	//return &tmp[0];
//}

void MeasureMenu( char cmd )
{
	ks0108DrawRoundRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, 10, BLACK);
}
