#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <avr/io.h>

#define F_CPU 16000000UL

typedef struct  
{
	unsigned update_fuel_values:1   ;
	unsigned update_total_fuel:1   ;
	unsigned update_time:1   ;
	unsigned update_menu:1   ;
	unsigned bit4:1   ;
	unsigned bit5:1   ;
	unsigned bit6:1   ;
	unsigned bit7:1   ;
} status_flags;

typedef struct
{
	char pressed_time;
	enum states {/*BS_UNKNOWN, */BS_PRESSED, BS_UNPRESSED, BS_LONGPRESSED} state;
} button_struct;
#define BUTTON_STRUCT_SIZE	sizeof(button_struct)

typedef struct 
{
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;	
}time_struct;
#define TIME_STRUCT_SIZE	sizeof(time_struct)

typedef struct 
{
	float consumption;
	time_struct time;
}measurement_struct;
#define MEASUREMENT_STRUCT_SIZE	sizeof(measurement_struct)


#define nop()   asm volatile ("nop")
#define sleep() asm volatile ("sleep")

#define sb(port, bit) (port |= _BV(bit))	//set bit
#define cb(port, bit) (port &= ~_BV(bit))	//clear bit
#define tb(port, bit) (port ^= _BV(bit))	//toggle bit
#define compare_bit(byte1, byte2, bit) ((!(_SFR_BYTE(byte1) & _BV(bit))) && (_SFR_BYTE(byte2) & _BV(bit)))

typedef uint8_t bool;

#define BTN_UP		4
#define BTN_RIGHT	5
#define BTN_LEFT	6
#define BTN_DOWN	7

//#define BT_PORT PORTC
//#define BT_DDR  DDRC
#define BT_PIN  PINC

#define DEBOUNCE	10//ms
#define LONG_PRESS	50

//количество импульсов в секунду при максимальном потоке 
//через датчик - литр в минуту
static const double IMPULSES_PER_GRAM_SECOND = (10000.0 / (60.0 * 60.0));
//static const float IMPULSES_PER_GRAM_SECOND = 160;//(1.0/6.0);

#define FALSE 0
#define TRUE 1

#define RECORDS_COUNT_ADDRESS	0

#endif /* GLOBAL_H_ */
