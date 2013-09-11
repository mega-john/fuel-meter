/*
 * FuelMeter.c
 *
 * Created: 15.08.2013 21:04:46
 *  Author: john
 */ 
#include "FuelMeter.h"
#include "timer.h"

volatile unsigned char in_fuel = 0;
volatile unsigned char out_fuel = 0;
volatile uint32_t total_fuel = 0;
volatile status_flags flags;
volatile time_struct ts; 
extern button_struct bs[4];

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
	DDRC=0xf0;
	PORTC=0x0f;

	// Port D initialization
	DDRD=0xff;
	PORTD=0xff;
}

int main(void)
{
	init_ports();
	init_int0();
	init_int1();
	init_timers();
	
	flags.update_fuel_values = 0;
	flags.update_time = 0;
	flags.update_total_fuel = 0;
	flags.update_menu = 1;
	flags.bit4 = 0;
	flags.bit5 = 0;
	flags.bit6 = 0;
	flags.bit7 = 0;
	
	ts.hours = 0;
	ts.minutes = 0;
	ts.seconds = 0;
	
	char tmp[20];

	//bs[UP].stat = BS_UNKNOWN;
	//bs[DOWN].stat = BS_UNKNOWN;
	//bs[LEFT].stat = BS_UNKNOWN;
	//bs[RIGHT].stat = BS_UNKNOWN;

	// Wait a little while the display starts up
	for(volatile uint16_t i = 0; i < 15000; i++);
	
	ts.hours = MEASUREMENT_STRUCT_SIZE;
	ts.minutes = BUTTON_STRUCT_SIZE;
	
	int count = ReadMeasurementsCount();
	count=0xabcd;
	WriteMeasurementsCount(count);
	// Initialize the LCD
	ks0108Init(count);
	
	menu_Init();
	
	sei();

	//ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
	//ks0108GotoXY(10 , 10);
	//ks0108Puts("Графические экраны, "); //пишем им


	MFPtr(0);

    while(1)
    {
		_delay_ms(2000);
		tb(PORTB, PINC5);
		//
		//set flag to 1
		//when button menu changes flag sets to 0
		//Flag = 1;
		//execute function that is pointed by FPtr
		MFPtr(0);

		
	//ks0108DrawCircle(25, 60, 20, WHITE);
		//if(flags.update_time == 1)
		//{
			//flags.update_time = 0;
			//if (++ts.seconds > 59)
			//{
				//ts.seconds = 0;
				//if(++ts.minutes > 59)
				//{
					//ts.minutes = 0;
					//if (++ts.hours > 23)
					//{
						//ts.hours = 0;
					//}
				//}
			//}
		//}
		//if(flags.update_fuel_values == 1)
		//{
			//flags.update_fuel_values = 0;
			//char total = in_fuel - out_fuel;
			//sprintf(tmp, "fuel  %u l/h", total);
		//}

		//itoa(in_fuel, tmp, 10);
		//tb(PORTC, PINC1);
		////LcdGotoXYFont(0,0);
		//LcdStr(FONT_1X, tmp);
   }
}