﻿#ifndef GLOBAL_H_
#define GLOBAL_H_

//#define DEBUG_IN_STUDIO

#include <avr/io.h>

typedef struct  
{
	unsigned update_fuel_values:1;
	unsigned update_total_fuel:1;
	unsigned update_time:1;
	unsigned update_menu:1;
	unsigned update_consumption:1;
	unsigned bit5:1;
	unsigned bit6:1;
	unsigned bit7:1;
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
	uint8_t day;
}time_struct;
#define TIME_STRUCT_SIZE	sizeof(time_struct)

typedef struct 
{
	double total;
	time_struct time;
}measurement_struct;
#define MEASUREMENT_STRUCT_SIZE	sizeof(measurement_struct)

#define nop()   asm volatile ("nop")
#define sleep() asm volatile ("sleep")

#define sb(port, bit) (port |= _BV(bit))	//set bit
#define cb(port, bit) (port &= ~_BV(bit))	//clear bit
#define tb(port, bit) (port ^= _BV(bit))	//toggle bit
#define compare_bit(byte1, byte2, bit) ((!(_SFR_BYTE(byte1) & _BV(bit))) && (_SFR_BYTE(byte2) & _BV(bit)))
#define CheckBit(port, bit) (port & _BV(bit))

#define BTN_UP		PIND4
#define BTN_RIGHT	PIND5
#define BTN_LEFT	PIND6
#define BTN_DOWN	PIND7
#define BTN_PIN		PIND
#define DEBOUNCE	10//x*16ms
#define LONG_PRESS	100//x*16ms

//количество импульсов в секунду при максимальном потоке 
//через датчик - литр в минуту
//static const double IMPULSES_PER_GRAM_SECOND = (10000.0 / (60.0 * 60.0));
#define IMPULSES_PER_GRAM_SECOND  (10000.0 / (60.0 * 60.0))

#define RECORDS_COUNT_ADDRESS	0x0000UL
#define FIRST_RECORD_ADDRESS	0x0010UL

#endif /* GLOBAL_H_ */