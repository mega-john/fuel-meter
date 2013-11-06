/*
 * FuelMeter.c
 *
 * Created: 15.08.2013 21:04:46
 *  Author: john
 
Type          FCH-m-POM-LC
Dimensions        (L x W x H) 58 x 41 x 27 mm
Impulse number      D1: 10000 Impulse/l, D3: 2500 Impulse/l
Operating voltage     5 - 24 Vdc
Connection        2 x 9.73 mm (G 1/8") + 2 x M5
Accuracy        ±2 %
Current consumption   8 mA
Measurement range     (with nozzle 1 mm) 0.01 - 1.0 l/min/(without nozzle, D = 3 mm) 0.05 - 3.5 l/min
Output current      20 mA
Material        POM, O-Ring: FKM
Pressure        Max 4 bar
Temperature range     -20 - +60 ∞C
 */ 
#include <string.h>

#include "FuelMeter.h"
#include "timer.h"

#include "hardware\wire\onewire.h"
#include "hardware\temperature\ds18x20.h"
#include "hardware\display\SystemRus5x7.h"

volatile uint8_t old_menu_index = 0;
volatile status_flags flags;
volatile time_struct ts; 
extern button_struct bs[4];
volatile double consumption = 0.0;
volatile Menu_State MN;
volatile uint16_t total_measurements = 0;;
measurement_struct ms1;
  
void init_ports(void)
{
  // Input/Output Ports initialization
  
  // Port A initialization
  DDRA = 0xff;
  PORTA = 0x00;
  
  // Port B initialization
  DDRB = 0xff;
  PORTB = 0xFF;

  // Port C initialization
  //если DDRx pin установлен в единицу, то вывод назначен как выход, если 0 то вход
  DDRC = 0b11111000;
  //If PORTxn is written logic one when the pin is configured as an input pin, the pull-up resistor is activated
  PORTC = 0b00000111;

  // Port D initialization
  DDRD = 0xf0;
  PORTD = 0xf0;
}

unsigned char nDevices; // количество сенсоров
unsigned char owDevicesIDs[MAXDEVICES][8];  // »х ID

unsigned char search_ow_devices(void) // поиск всех устройств на шине
{
  unsigned char i;
  unsigned char id[OW_ROMCODE_SIZE];
  unsigned char diff, sensors_count;

  sensors_count = 0;

  for( diff = OW_SEARCH_FIRST; diff != OW_LAST_DEVICE && sensors_count < MAXDEVICES ; )
  {
    OW_FindROM( &diff, &id[0] );

    if( diff == OW_PRESENCE_ERR ) break;

    if( diff == OW_DATA_ERR ) break;

    for (i = 0; i < OW_ROMCODE_SIZE; i++)
    owDevicesIDs[sensors_count][i] = id[i];
    
    sensors_count++;
  }
  return sensors_count;

}

int main(void)
{
	init_ports();
	init_ext_interrupts();
	init_timers();
	eeInit();
	ks0108Init(0);
	// Wait a little while the display starts up
	for(volatile uint16_t i = 0; i < 15000; i++);
	//_delay_ms(500); 
  
	sei();

	TFT_init();
	menu_init();    

	//char tmp[100] = "’÷„ЎўЏџ№Ёёяабвгдежзий\0";
	char tmp[100] = "qwertyuiop\0";
	//memset(&tmp, 0xff, 100);
	//
	//uint16_t addr = 100;
	//eeWriteBytes(0, (void*)&tmp, 100);
	//eeWriteBytes(100, (void*)&tmp, 100);
	//eeWriteBytes(200, (void*)&tmp, 100);
	//eeWriteBytes(300, (void*)&tmp, 100);

	ks0108SelectFont(SystemRus5x7, ks0108ReadFontData, BLACK);
	//ks0108DrawRoundRect(0, 0, 127, SC_HEIGHT, 4, BLACK);
	//ks0108GotoXY(0, 0);
	//ks0108Puts("header");
	_delay_ms(2000);
	//uint8_t data[7] = {0, 23, 23, 6, 6, 10 , 13};
	//ds1703_write((uint8_t*)&data);
//
	nDevices = search_ow_devices(); // ищем все устройства
  
  //ks0108Puts((char*)&tmp);
  /*while(1)*/
	ks0108GotoXY(0, 0);
	sprintf(tmp, "found: %i", nDevices);
	ks0108Puts((char*)&tmp[0]);
	_delay_ms(1000);
    //sprintf(tmp, "клмнопрстуфхцчшщъыьэ#", nDevices);
    //ks0108Puts((char*)&tmp);
    //ks0108GotoXY(0, 30);
    //sprintf(tmp, "%i ю€123567890!\"є;%:?*()_+", nDevices);
    //ks0108Puts((char*)&tmp);
	//while(1);
	//ks0108ClearScreen();
	//while(1)   
	//{
		//for (unsigned char i = 0; i < nDevices; i++) // теперь сотируем устройства и запрашиваем данные
		//{
		//// узнать устройство можно по его груповому коду, который расположен в первом байте адресса
			//switch (owDevicesIDs[i][0])
			//{
				//case OW_DS18B20_FAMILY_CODE: 
				//{ // если найден термодатчик DS18B20
					////printf("\r"); print_address(owDevicesIDs[i]); // печатаем знак переноса строки, затем - адрес
					////printf(" - Thermometer DS18B20"); // печатаем тип устройства
					//DS18x20_StartMeasureAddressed(owDevicesIDs[i]); // запускаем измерение
					////timerDelayMs(800); // ждем минимум 750 мс, пока конвентируетс€ температура
					//_delay_ms(800); // ждем минимум 750 мс, пока конвентируетс€ температура
					//unsigned char data[2]; // переменна€ дл€ хранени€ старшего и младшего байта данных
					//DS18x20_ReadData(owDevicesIDs[i], data); // считываем данные
					//unsigned char themperature[3]; // в этот массив будет записана температура
					//DS18x20_ConvertToThemperature(data, themperature); // преобразовываем температуру в человекопон€тный вид
					//ks0108GotoXY(0, 10 * i);
					//ks0108FillRect(ks0108StringWidth("value: "), 10 * i, 10, 12, WHITE);
					//sprintf(tmp, "value: %c%d.%1d C", themperature[0], themperature[1], themperature[2]);
					//ks0108Puts((char*)&tmp);
				//} break;        
			//}
		//};
	//}
//
  
  total_measurements = 0x0;
  //WriteMeasurementsCount();
//////  _delay_ms(5);
  ReadMeasurementsCount();
  //_delay_ms(5);
  //ReadMeasurement(0, &ms1) ;
  //ms1.magic++;
  //_delay_ms(5);
  //ReadMeasurement(1, &ms1) ;
  //ms1.magic++;
  //
  //
  //for (uint8_t i = 0; i < 20; i++)
  //{
    //ms1.total = 7.5 + i;
    //ms1.time.hours = 1;
    //ms1.time.minutes = 15;
    //ms1.time.seconds = 0;
    //ms1.time.day = 0;
    //WriteMeasurement(&ms1);
  //}
  
    while(1)
    {
		if(old_menu_index != MN.menuNo)
		{
		  ks0108ClearScreen();
		  old_menu_index = MN.menuNo;
		}
		MFPtr(0);   
   }
}