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
Accuracy 				�2 %
Current consumption 	8 mA
Measurement range 		(with nozzle 1 mm) 0.01 - 1.0 l/min/(without nozzle, D = 3 mm) 0.05 - 3.5 l/min
Output current 			20 mA
Material 				POM, O-Ring: FKM
Pressure 				Max 4 bar
Temperature range 		-20 - +60 �C
 */ 
#include "FuelMeter.h"
#include "timer.h"

volatile uint8_t in_fuel = 0;
volatile uint8_t out_fuel = 0;
volatile double total_fuel = 0;
volatile status_flags flags;
volatile time_struct ts; 
extern button_struct bs[4];
//volatile float f = 0.1666;
volatile double consumption = 0.0;

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
	DDRC=0x0f;
	PORTC=0xf0;

	// Port D initialization
	DDRD=0xff;
	PORTD=0x00;
}

inline uint32_t ToSeconds(time_struct* ts)
{
	return ts->seconds + ts->minutes*60 + ts->hours*3600;
}

int main(void)
{
	init_ports();
	init_int0();
	init_int1();
	
	ts.hours = 0;
	ts.minutes = 0;
	ts.seconds = 0;
	
	//char tmp[20];

	//test.time.mili_seconds = 1;
	//test.time.seconds = 1;
	//test.time.minutes = 1;
	//test.time.hours = 1;
//
	//uint32_t tt = test.m;
		//	consumption = IMPULSES_PER_GRAM_SECOND + 0.1;
	
	//bs[UP].stat = BS_UNKNOWN;
	//bs[DOWN].stat = BS_UNKNOWN;
	//bs[LEFT].stat = BS_UNKNOWN;
	//bs[RIGHT].stat = BS_UNKNOWN;

	// Wait a little while the display starts up
	//for(volatile uint16_t i = 0; i < 15000; i++);
	
	//ts.hours = MEASUREMENT_STRUCT_SIZE;
	//ts.minutes = BUTTON_STRUCT_SIZE;
	//
	//int count = ReadMeasurementsCount();
	//count=0xabcd;
	//WriteMeasurementsCount(count);
	//
	//f = GRAMMS_PER_SECOND;
	//WriteMeasurementsCount((uint16_t)f);
//
	// Initialize the LCD
	ks0108Init(0);
	
	menu_Init();
	
	init_timers();
	
	//eeInit();
	sei();

	ks0108GotoXY(0 , 0);
	ks0108ClearScreen();
	ks0108SelectFont(SC, ks0108ReadFontData, BLACK);
ks0108FillRect(ks0108StringWidth("total  "), 16, 70, 9, WHITE);
	uint8_t mem = 0;
	uint8_t addr = 0;
	//MFPtr(0);

	//eeWriteBytes(addr, (uint8_t*)&tmp, 20);

    while(1)
    {
		//_delay_ms(2000);
		//tb(PORTB, PINC5);
		////
		//set flag to 1
		//when button menu changes flag sets to 0
		//Flag = 1;
		//execute function that is pointed by FPtr
		
		if(flags.update_fuel_values == 1)
		{
			//ks0108ClearScreen();
			//uint8_t total = in_fuel - out_fuel;
			//consumption = total / IMPULSES_PER_GRAM_SECOND;
			//ks0108GotoXY(0, 0);
			//sprintf(tmp, "consumption  %.2f L/h", consumption);
			//ks0108Puts(tmp);
//
			//total_fuel += (consumption / 3600);
			//ks0108GotoXY(0, 16);
			//ks0108FillRect(ks0108StringWidth("total  "), 16, 70, 9, WHITE);
			//ks0108GotoXY(0, 16);
			//sprintf(tmp, "total  %.2f L", (total_fuel));
			//ks0108Puts(tmp);
//
			//ks0108GotoXY(0, 32);
			//sprintf(tmp, "work time %02u:%02u:%02u", ts.hours, ts.minutes, ts.seconds);
			//ks0108Puts(tmp);
			MFPtr(MN.menuNo);

			flags.update_fuel_values = 0;
		}
   }
}