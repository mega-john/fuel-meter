/*
 * FuelMeter.c
 *
 * Created: 15.08.2013 21:04:46
 *  Author: john
 
Type 					FCH-m-POM-LC
Dimensions 				(L x W x H) 58 x 41 x 27 mm
Impulse number 			D1: 10000 Impulse/l, D3: 2500 Impulse/l
Operating voltage 		5 - 24 Vdc
Connection 				2 x 9.73 mm (G 1/8") + 2 x M5
Accuracy 				±2 %
Current consumption 	8 mA
Measurement range 		(with nozzle 1 mm) 0.01 - 1.0 l/min/(without nozzle, D = 3 mm) 0.05 - 3.5 l/min
Output current 			20 mA
Material 				POM, O-Ring: FKM
Pressure 				Max 4 bar
Temperature range 		-20 - +60 °C
 */ 
#include <string.h>

#include "FuelMeter.h"
#include "timer.h"

volatile uint8_t old_menu_index = 0;
//volatile uint8_t in_fuel = 0;
//volatile uint8_t out_fuel = 0;
//volatile double total_fuel_value = 0;
volatile status_flags flags;
volatile time_struct ts; 
extern button_struct bs[4];
//volatile float f = 0.1666;
volatile double consumption = 0.0;
volatile Menu_State MN;
volatile uint16_t total_measurements = 0;;
measurement_struct ms1;


void init_ports(void)
{
	// Input/Output Ports initialization
	
	// Port A initialization
	DDRA=0xff;
	PORTA=0x00;
	
	// Port B initialization
	DDRB=0xff;
	PORTB=0xFF;

	// Port C initialization
	//если DDRx pin установлен в единицу, то вывод назначен как выход, если 0 то вход
	DDRC=0x02;
	PORTC=0xff;

	// Port D initialization
	DDRD=0x00;
	PORTD=0xff;
}

int main(void)
{
	init_ports();
	init_ext_interrupts();
	init_timers();
	eeInit();
	ks0108Init(0);
		
	ts.hours = 0;
	ts.minutes = 0;
	ts.seconds = 0;
	
	// Wait a little while the display starts up
	//for(volatile uint16_t i = 0; i < 15000; i++);
		
	sei();

	menu_init();
	
	//uint8_t tmp[100];
	//memset(&tmp, 0xff, 100);
	//
	//uint16_t addr = 100;
	//eeWriteBytes(0, (void*)&tmp, 100);
	//eeWriteBytes(100, (void*)&tmp, 100);
	//eeWriteBytes(200, (void*)&tmp, 100);
	//eeWriteBytes(300, (void*)&tmp, 100);

		
	total_measurements = 0x0;
	//WriteMeasurementsCount();
//////	_delay_ms(5);
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