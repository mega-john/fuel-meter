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

������������ ����� � ��������, ����� ����� �������� 10000 ��������� � ������ �� ����, ��� 166.6 � �������.
���� ����� ������� ��������� ��� � �������, �� ������ ������ ���������� ���� uint8_t
�� ����� ���������� 0,1666666666666667
 */ 


#include "FuelMeter.h"
#include "timer.h"

volatile uint8_t in_fuel = 0;
volatile uint8_t out_fuel = 0;
volatile double total_fuel = 0;
volatile status_flags flags;
volatile time_struct ts; 
extern button_struct bs[4];
volatile 	float f = 0.1666;
volatile	double consumption = 0.0;

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
	sei();

	ks0108SelectFont(SC, ks0108ReadFontData, BLACK);
	//ks0108GotoXY(10 , 10);
	//ks0108Puts("fuel meter"); //����� ��


	MFPtr(0);

    while(1)
    {
		//_delay_ms(2000);
		//tb(PORTB, PINC5);
		////
		//set flag to 1
		//when button menu changes flag sets to 0
		//Flag = 1;
		//execute function that is pointed by FPtr
		//MFPtr(0);

		
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
		if(flags.update_fuel_values == 1)
		{
			ks0108ClearScreen();
			uint8_t total = in_fuel - out_fuel;
			consumption = total / IMPULSES_PER_GRAM_SECOND;
			ks0108GotoXY(0, 0);
			sprintf(tmp, "consumption  %.2f l/h", consumption);
			ks0108Puts(tmp);

			total_fuel += consumption;
			ks0108GotoXY(0, 16);
			sprintf(tmp, "total  %.2fl l/h", total_fuel);
			ks0108Puts(tmp);

			ks0108GotoXY(0, 32);
			sprintf(tmp, "work time %02u:%02u:%02u", ts.hours, ts.minutes, ts.seconds);
			ks0108Puts(tmp);
			flags.update_fuel_values = 0;
		}

		//itoa(in_fuel, tmp, 10);
		//tb(PORTC, PINC1);
		////LcdGotoXYFont(0,0);
		//LcdStr(FONT_1X, tmp);
   }
}